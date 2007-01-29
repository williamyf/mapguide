//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "FdoConnectionManager.h"
#include "System/XmlDefs.h"
#include "System/XmlUtil.h"
#include "ServiceManager.h"
#include "LogManager.h"
#include "LongTransactionManager.h"
#include "UnmanagedDataManager.h"

ACE_Recursive_Thread_Mutex MgFdoConnectionManager::sm_mutex;

const INT32 MaxFdoConnectionPoolSize = 1000;

// Process-wide MgFdoConnectionManager
Ptr<MgFdoConnectionManager> MgFdoConnectionManager::sm_fdoConnectionManager = (MgFdoConnectionManager*)NULL;


// Constructor
MgFdoConnectionManager::MgFdoConnectionManager(void)
{
    m_connManager = NULL;

    m_bFdoConnectionPoolEnabled = false;
    m_nFdoConnectionPoolSize = 0;
    m_nFdoConnectionTimeout = 0;
}


// Destructor
MgFdoConnectionManager::~MgFdoConnectionManager(void)
{
    MG_FDOCONNECTION_MANAGER_TRY()

    ClearCache();

    FDO_SAFE_RELEASE(m_connManager);

    MG_FDOCONNECTION_MANAGER_CATCH(L"MgFdoConnectionManager.~MgFdoConnectionManager")
}


void MgFdoConnectionManager::Dispose(void)
{
    delete this;
}

// Get pointer to a process-wide MgFdoConnectionManager.
MgFdoConnectionManager* MgFdoConnectionManager::GetInstance()
{
    MG_FDOCONNECTION_MANAGER_TRY()

    ACE_TRACE ("MgFdoConnectionManager::GetInstance");

    if (MgFdoConnectionManager::sm_fdoConnectionManager == NULL)
    {
        // Perform Double-Checked Locking Optimization.
        ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance (), 0));
        if (MgFdoConnectionManager::sm_fdoConnectionManager == NULL)
        {
            MgFdoConnectionManager::sm_fdoConnectionManager = new MgFdoConnectionManager;
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.GetInstance")

    // To avoid overheads and maintain thread safety,
    // do not assign this returned static singleton to a Ptr object.
    return MgFdoConnectionManager::sm_fdoConnectionManager;
}


///----------------------------------------------------------------------------
/// <summary>
/// Intializes the FDO connection manager.
/// This method must be called once during the server startup time.
/// </summary>
///----------------------------------------------------------------------------

void MgFdoConnectionManager::Initialize(bool bFdoConnectionPoolEnabled, INT32 nFdoConnectionPoolSize, INT32 nFdoConnectionTimeout, STRING excludedProviders)
{
    MG_FDOCONNECTION_MANAGER_TRY()

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager::Initialize()\n")));
    MG_LOG_TRACE_ENTRY(L"MgFdoConnectionManager::Initialize()");

    m_connManager = FdoFeatureAccessManager::GetConnectionManager();
    CHECKNULL(m_connManager, L"MgFdoConnectionManager.Initialize()");

    if((MaxFdoConnectionPoolSize < nFdoConnectionPoolSize) || (1 > nFdoConnectionPoolSize))
    {
        nFdoConnectionPoolSize = MaxFdoConnectionPoolSize;
    }

    m_bFdoConnectionPoolEnabled = bFdoConnectionPoolEnabled;
    m_nFdoConnectionPoolSize = nFdoConnectionPoolSize;
    m_nFdoConnectionTimeout = nFdoConnectionTimeout;

    // Parse the comma delimited string into a string collection
    m_excludedProviders = MgStringCollection::ParseCollection(excludedProviders, L",");

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.Initialize")
}


///----------------------------------------------------------------------------
/// <summary>
/// Cleans up the FDO connection manager.
/// This method must be called once during the server shutdown time.
/// </summary>
///----------------------------------------------------------------------------

void MgFdoConnectionManager::Terminate()
{
    MgFdoConnectionManager::sm_fdoConnectionManager = NULL;
}


FdoIConnection* MgFdoConnectionManager::Open(MgResourceIdentifier* resourceIdentifier)
{
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, NULL));

    FdoPtr<FdoIConnection> pFdoConnection;

    MG_FDOCONNECTION_MANAGER_TRY()

    ACE_TRACE ("MgFdoConnectionManager::Open");

    if(m_bFdoConnectionPoolEnabled)
    {
        // Search the cache for an FDO connection matching this resourceIdentifier
        // The content and long transaction name must also match, as the information may change
        pFdoConnection = FindFdoConnection(resourceIdentifier);
    }

    if(NULL == pFdoConnection)
    {
        // Retrieve XML from repository
        string featureSourceXmlContent;
        RetrieveFeatureSource(resourceIdentifier, featureSourceXmlContent);

        // Parse XML and get properties

        auto_ptr<MdfModel::FeatureSource> featureSource(GetFeatureSource(resourceIdentifier));

        STRING providerName = (STRING)featureSource->GetProvider();
        STRING configDocumentName = (STRING)featureSource->GetConfigurationDocument();
        STRING longTransactionName = (STRING)featureSource->GetLongTransaction();

        providerName = UpdateProviderName(providerName);

        // Update the long transaction name to any active one for the current request
        MgLongTransactionManager::GetLongTransactionName(resourceIdentifier, longTransactionName);

        // Create a new connection and add it to the cache
        pFdoConnection = m_connManager->CreateConnection(providerName.c_str());

        // Retrieve the properties and open the connection
        SetConnectionProperties(pFdoConnection, featureSource.get());

        SetConfiguration(providerName, pFdoConnection, resourceIdentifier, configDocumentName);

        Open(pFdoConnection);

        // The following are only applicable when we are creating from a resource identifier
        ActivateLongTransaction(pFdoConnection, longTransactionName);

        if(m_bFdoConnectionPoolEnabled)
        {
            // Check to see if this provider has been excluded from caching
            if(!IsExcludedProvider(providerName))
            {
                if(!FdoConnectionCacheFull())
                {
                    // Add this entry to the cache
                    CacheFdoConnection(pFdoConnection, resourceIdentifier->ToString(),
                                       featureSourceXmlContent,
                                       longTransactionName);
                }
            }
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.Open")

    return pFdoConnection.Detach();
}


FdoIConnection* MgFdoConnectionManager::Open(CREFSTRING providerName, CREFSTRING connectionString)
{
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, NULL));

    FdoPtr<FdoIConnection> pFdoConnection;

    MG_FDOCONNECTION_MANAGER_TRY()

    ACE_TRACE ("MgFdoConnectionManager::Open");

    // Connection string should have something.

    // Empty connection string is allowed for ODBC provider to retrieve
    // DataSources
    if (providerName.empty())
    {
        MgStringCollection arguments;
        arguments.Add(L"1");
        arguments.Add(MgResources::BlankArgument);

        throw new MgInvalidArgumentException(L"MgFdoConnectionManager.Open",
            __LINE__, __WFILE__, &arguments, L"MgStringEmpty", NULL);
    }

    STRING providerNameNoVersion = UpdateProviderName(providerName);

    if(m_bFdoConnectionPoolEnabled)
    {
        // Search the cache for an FDO connection matching this provider/connection string
        pFdoConnection = FindFdoConnection(providerNameNoVersion, connectionString);
    }

    if(NULL == pFdoConnection)
    {
        // Create a new connection and add it to the cache
        pFdoConnection = m_connManager->CreateConnection(providerNameNoVersion.c_str());
        // No connection string, no pooling and connection will remain in closed state
        if (!connectionString.empty())
        {
            // Set the connection properties
            pFdoConnection->SetConnectionString(connectionString.c_str());

            // Open the connection to the FDO provider
            Open(pFdoConnection);

            if(m_bFdoConnectionPoolEnabled)
            {
                // Check to see if this provider has been excluded from caching
                if(!IsExcludedProvider(providerNameNoVersion))
                {
                    if(!FdoConnectionCacheFull())
                    {
                        // Add this entry to the cache
                        STRING key = providerNameNoVersion + L" - " + connectionString;
                        string data = "";
                        STRING ltName = L"";
                        CacheFdoConnection(pFdoConnection, key, data, ltName);
                    }
                }
            }
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.Open")

    return pFdoConnection.Detach();
}


void MgFdoConnectionManager::Close(FdoIConnection* pFdoConnection)
{
    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex));

    CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.Close()");

    MG_FDOCONNECTION_MANAGER_TRY()

    // Release reference
    FDO_SAFE_RELEASE(pFdoConnection);

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.Close")
}


// TODO: Need to tie in MgServerFeatureReaderIdentifierPool and any other pools when we remove cached connection and close it
void MgFdoConnectionManager::RemoveExpiredConnections()
{
    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex));

    MG_FDOCONNECTION_MANAGER_TRY()

    ACE_Time_Value now = ACE_OS::gettimeofday();
    FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

    while(m_FdoConnectionCache.end() != iter)
    {
        FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
        if(pFdoConnectionCacheEntry)
        {
            INT32 time = now.sec() - pFdoConnectionCacheEntry->lastUsed.sec();
            if(time > m_nFdoConnectionTimeout)
            {
                // Connection has expired so close it and remove it
                pFdoConnectionCacheEntry->pFdoConnection->Close();

                // Release any resource
                FDO_SAFE_RELEASE(pFdoConnectionCacheEntry->pFdoConnection);

                delete pFdoConnectionCacheEntry;
                pFdoConnectionCacheEntry = NULL;

                STRING cacheKey = iter->first;

                // Remove any feature service cache entries for this resource
                MgServiceManager* serviceManager = MgServiceManager::GetInstance();
                assert(NULL != serviceManager);

                try
                {
                    Ptr<MgResourceIdentifier> resource = new MgResourceIdentifier(cacheKey);
                    serviceManager->RemoveFeatureServiceCacheEntry(resource);
                }
                catch(MgInvalidRepositoryTypeException* e)
                {
                    // If this exception is thrown then the key was not a resource identifier string 
                    // and so there will be no entries in the feature service cache to remove.
                    SAFE_RELEASE(e);
                }

                m_FdoConnectionCache.erase(iter++);

                ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager.RemoveExpiredConnections() - Found expired cached FDO connection.\n")));
            }
            else
            {
                // Check the next cached connection
                iter++;
            }
        }
        else
        {
            // NULL Pointer
            break;
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.RemoveExpiredConnections")
}


FdoIConnection* MgFdoConnectionManager::FindFdoConnection(MgResourceIdentifier* resourceIdentifier)
{
    CHECKNULL(resourceIdentifier, L"MgFdoConnectionManager.FindFdoConnection()");

    FdoIConnection* pFdoConnection = NULL;

    MG_FDOCONNECTION_MANAGER_TRY()

    // Retrieve XML from repository
    string featureSourceXmlContent;
    RetrieveFeatureSource(resourceIdentifier, featureSourceXmlContent);

    // Get the active long transaction name for the current request
    STRING ltName = L"";

    if(!MgLongTransactionManager::GetLongTransactionName(resourceIdentifier, ltName))
    {
        // No long transaction name cached for the current session or no current session
        // In this case we want to use the requested long transaction of the feature source

        // Parse feature source XML and get long transaction

        auto_ptr<MdfModel::FeatureSource> featureSource(GetFeatureSource(resourceIdentifier));

        STRING longTransactionName = (STRING)featureSource->GetLongTransaction();

        ltName = longTransactionName;
    }

    pFdoConnection = SearchFdoConnectionCache(resourceIdentifier->ToString(),
                                              featureSourceXmlContent,
                                              ltName);

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.FindFdoConnection")

    return pFdoConnection;
}


FdoIConnection* MgFdoConnectionManager::FindFdoConnection(CREFSTRING providerName, CREFSTRING connectionString)
{
    FdoIConnection* pFdoConnection = NULL;

    MG_FDOCONNECTION_MANAGER_TRY()

    STRING providerNameNoVersion = UpdateProviderName(providerName);
    STRING key = providerNameNoVersion + L" - " + connectionString;
    string data = "";
    STRING ltName = L"";

    pFdoConnection = SearchFdoConnectionCache(key, data, ltName);

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.FindFdoConnection")

    return pFdoConnection;
}


FdoIConnection* MgFdoConnectionManager::SearchFdoConnectionCache(CREFSTRING key, string& data, CREFSTRING ltName)
{
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, NULL));

    FdoIConnection* pFdoConnection = NULL;

    MG_FDOCONNECTION_MANAGER_TRY()

    FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

    while(m_FdoConnectionCache.end() != iter)
    {
        STRING cacheKey = iter->first;
        if(ACE_OS::strcasecmp(cacheKey.c_str(), key.c_str()) == 0)
        {
            // We have a key match
            FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
            if(pFdoConnectionCacheEntry)
            {
                if(pFdoConnectionCacheEntry->data == data)
                {
                    // We have a data match
                    if(pFdoConnectionCacheEntry->ltName == ltName)
                    {
                        // We have a long transaction name match
                        if(pFdoConnectionCacheEntry->pFdoConnection->GetRefCount() == 1)
                        {
                            // It is not in use so claim it
                            pFdoConnectionCacheEntry->lastUsed = ACE_OS::gettimeofday();
                            if (FdoConnectionState_Closed == pFdoConnectionCacheEntry->pFdoConnection->GetConnectionState())
                            {
                                pFdoConnectionCacheEntry->pFdoConnection->Open();
                            }
                            pFdoConnection = FDO_SAFE_ADDREF(pFdoConnectionCacheEntry->pFdoConnection);
                            break;
                        }
                    }
                }
            }
        }

        iter++;
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.SearchFdoConnectionCache")

    #ifdef _DEBUG
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SearchFdoConnectionCache:\nConnection: %@\nKey = %W\nData = %W\nVersion(LT) = %W\n\n"), (void*)pFdoConnection, key.c_str(), data.empty() ? L"(empty)" : L"(data)", ltName.empty() ? L"(empty)" : ltName.c_str()));
    #endif

    return pFdoConnection;
}

void MgFdoConnectionManager::GetSpatialContextInfoFromXml(MdfModel::FeatureSource* pFeatureSource, MgSpatialContextInfoMap* spatialContextInfoMap)
{
    CHECKNULL(pFeatureSource, L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");

    MG_FDOCONNECTION_MANAGER_TRY()

    MdfModel::SupplementalSpatialContextInfoCollection* spatialContexts = pFeatureSource->GetSupplementalSpatialContextInfo();
    CHECKNULL(spatialContexts, L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");

    for (int i = 0; i < spatialContexts->GetCount(); i++)
    {
        MdfModel::SupplementalSpatialContextInfo* spatialContext = spatialContexts->GetAt(i);
        CHECKNULL(spatialContext, L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");

        // Name element
        STRING name = (STRING)spatialContext->GetName();
        FdoString* propertyName = name.c_str();
        CHECKNULL(propertyName, L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");

        // CoordinateSystem element
        STRING coordinateSystem = (STRING)spatialContext->GetCoordinateSystem();
        FdoString* propertyCoordinateSystem = coordinateSystem.c_str();
        CHECKNULL(propertyCoordinateSystem, L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");

        spatialContextInfoMap->insert( MgSpatialContextInfoPair(name, coordinateSystem) );
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.GetSpatialContextInfoFromXml");    
}

void MgFdoConnectionManager::SetConnectionProperties(FdoIConnection *pFdoConnection, MdfModel::FeatureSource *pFeatureSource)
{
        CHECKNULL(pFeatureSource, L"MgFdoConnectionManager.SetConnectionProperties");
        CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.SetConnectionProperties");

        // Get FdoIConnectionInfo
        FdoPtr<FdoIConnectionInfo> fdoConnInfo = pFdoConnection->GetConnectionInfo();
        CHECKNULL((FdoIConnectionInfo*)fdoConnInfo, L"MgFdoConnectionManager.SetConnectionProperties");

        // GetFdoIConnectionPropertyDictionary
        FdoPtr<FdoIConnectionPropertyDictionary> fdoConnPropertyDict = fdoConnInfo->GetConnectionProperties();
        CHECKNULL((FdoIConnectionPropertyDictionary*)fdoConnPropertyDict, L"MgFdoConnectionManager.SetConnectionProperties");

        // Get all all connection properties
        MdfModel::NameStringPairCollection* parameters = pFeatureSource->GetParameters();
        CHECKNULL(parameters, L"MgFdoConnectionManager.SetConnectionProperties");

        for (int i = 0; i < parameters->GetCount(); i++)
        {
            // Get the Name and Value elements
            MdfModel::NameStringPair* pair = parameters->GetAt(i);
            STRING name = (STRING)pair->GetName();
            STRING value = (STRING)pair->GetValue();

            // If name is null, means invalid xml
            if (name.empty())
            {
                STRING message = MgUtil::GetResourceMessage(MgResources::FeatureService, L"MgInvalidPropertyName");

                Ptr<MgStringCollection> strCol;
                if (!message.empty())
                {
                    strCol = new MgStringCollection();
                    strCol->Add(message);
                }

                throw new MgInvalidFeatureSourceException(L"MgFdoConnectionManager.SetConnectionProperties",
                    __LINE__, __WFILE__, (MgStringCollection*)strCol, L"", NULL);
            }

            FdoString* propertyName = name.c_str();
            CHECKNULL(propertyName, L"MgFdoConnectionManager.SetConnectionProperties");

            // Property value can be null ( optional properties may not have values )
            if (!value.empty())
            {
                // if it's a "File" property, check for unmanaged data alias
                // and replace the alias with the mapped value
                if (name.compare(L"File") == 0)
                    MgUnmanagedDataManager::ConvertUnmanagedDataMappingName(value);

                FdoString* propertyValue = value.c_str();
                if (propertyValue != NULL)
                {
                    fdoConnPropertyDict->SetProperty(propertyName, propertyValue);
                }
            }
        }
}

void MgFdoConnectionManager::ActivateSpatialContext(FdoIConnection* pFdoConnection, STRING& spatialContextName)
{
    CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.ActivateSpatialContext()");
    // If command is not supported we simply return and ignore element from xml
    if (!SupportsCommand(pFdoConnection, FdoCommandType_ActivateSpatialContext))
    {
        return;
    }

    // No spatial context specified
    if (spatialContextName.length() <=0)
    {
        return;
    }

    // Spatial Context can be executed when connection is already open
    if (pFdoConnection->GetConnectionState() == FdoConnectionState_Open)
    {
        FdoPtr<FdoIActivateSpatialContext> fdoCommand = (FdoIActivateSpatialContext*)pFdoConnection->CreateCommand(FdoCommandType_ActivateSpatialContext);
        CHECKNULL((FdoIActivateSpatialContext*)fdoCommand, L"MgFdoConnectionManager.ActivateSpatialContext");
        // Set the spatial context from the feature source
        fdoCommand->SetName(spatialContextName.c_str());
        // Execute the command to activate
        fdoCommand->Execute();
    }
}


void MgFdoConnectionManager::ActivateLongTransaction(FdoIConnection* pFdoConnection, STRING& longTransactionName)
{
    CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.ActivateLongTransaction()");

    // If command is not supported we simply return and ignore element from xml
    if (!SupportsCommand(pFdoConnection, FdoCommandType_ActivateLongTransaction))
    {
        return;
    }

    // No long transaction specified
    if (longTransactionName.length() <=0)
    {
        return;
    }

    // Long transaction can be executed when connection is already open
    if (pFdoConnection->GetConnectionState() == FdoConnectionState_Open)
    {
        FdoPtr<FdoIActivateLongTransaction> fdoCommand = (FdoIActivateLongTransaction*)pFdoConnection->CreateCommand(FdoCommandType_ActivateLongTransaction);
        CHECKNULL((FdoIActivateLongTransaction*)fdoCommand, L"MgFdoConnectionManager.ActivateLongTransaction");
        // Set the spatial context from the feature source
        fdoCommand->SetName(longTransactionName.c_str());
        // Execute the command to activate
        fdoCommand->Execute();
    }
}


bool MgFdoConnectionManager::SupportsConfiguration(FdoIConnection* pFdoConnection)
{
    CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.SupportsConfiguration()");

    FdoPtr<FdoIConnectionCapabilities> ficc = pFdoConnection->GetConnectionCapabilities();
    CHECKNULL((FdoIConnectionCapabilities*)ficc, L"MgFdoConnectionManager.SupportsConfiguration");

    return ficc->SupportsConfiguration();
}


void MgFdoConnectionManager::RetrieveFeatureSource(MgResourceIdentifier* resource, string& resourceContent)
{
    CHECKNULL(resource, L"MgFdoConnectionManager.RetrieveFeatureSource");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex));

    MG_FDOCONNECTION_MANAGER_TRY()

    FdoConnectionCacheEntry* pFdoConnectionCacheEntry = NULL;
    resourceContent = "";

    // Check to see if we have a valid resource
    bool bValidResource = false;
    try
    {
        resource->Validate();
        bValidResource = true;
    }
    catch(MgException* e)
    {
        SAFE_RELEASE(e);
    }

    if(bValidResource)
    {
        STRING key = resource->ToString();

        FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

        while(m_FdoConnectionCache.end() != iter)
        {
            STRING cacheKey = iter->first;
            if(ACE_OS::strcasecmp(cacheKey.c_str(), key.c_str()) == 0)
            {
                // We have a key match
                pFdoConnectionCacheEntry = iter->second;
                if(pFdoConnectionCacheEntry)
                {
                    resourceContent = pFdoConnectionCacheEntry->data;
                    break;
                }
            }

            iter++;
        }
    }

    if(resourceContent.empty())
    {
        MgServiceManager* serviceMan = MgServiceManager::GetInstance();
        assert(NULL != serviceMan);

        // Get the service from service manager
        Ptr<MgResourceService> resourceService = dynamic_cast<MgResourceService*>(
            serviceMan->RequestService(MgServiceType::ResourceService));
        assert(resourceService != NULL);

        // Get the feature source contents
        Ptr<MgByteReader> byteReader = resourceService->GetResourceContent(resource, MgResourcePreProcessingType::Substitution);

        Ptr<MgByteSink> byteSink = new MgByteSink((MgByteReader*)byteReader);
        byteSink->ToStringUtf8(resourceContent);

        ValidateFeatureSource(resourceContent);

        if(pFdoConnectionCacheEntry)
        {
            // Update the cache entry with the resource document now that we have it
            pFdoConnectionCacheEntry->data = resourceContent;
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.RetrieveFeatureSource")
}


void MgFdoConnectionManager::SetConfiguration(CREFSTRING providerName, FdoIConnection* pFdoConnection, MgResourceIdentifier* resourceIdentifier, STRING& configDataName)
{
    CHECKNULL(resourceIdentifier, L"MgFdoConnectionManager.SetConfiguration");
    CHECKNULL(pFdoConnection, L"MgFdoConnectionManager.SetConfiguration");

    if (providerName.empty())
    {
        MgStringCollection arguments;
        arguments.Add(L"1");
        arguments.Add(MgResources::BlankArgument);

        throw new MgInvalidArgumentException(L"MgFdoConnectionManager.SetConfiguration",
            __LINE__, __WFILE__, &arguments, L"MgStringEmpty", NULL);
    }

    if (!SupportsConfiguration(pFdoConnection))
    {
        // Configuration is not supported
        return;
    }

    // No configration specified. Defaults to provider configuration
    if (configDataName.length() <= 0)
    {
        return;
    }

    MgServiceManager* serviceMan = MgServiceManager::GetInstance();
    assert(NULL != serviceMan);

    // Get the service from service manager
    Ptr<MgResourceService> resourceService = dynamic_cast<MgResourceService*>(
        serviceMan->RequestService(MgServiceType::ResourceService));
    assert(resourceService != NULL);

    // Get the feature source contents
    Ptr<MgByteReader> byteReader = resourceService->GetResourceData(resourceIdentifier, configDataName , MgResourcePreProcessingType::Substitution);
    if (byteReader == NULL)
    {
        STRING message = MgUtil::GetResourceMessage(MgResources::FeatureService, L"MgMissingConfiguration");

        Ptr<MgStringCollection> strCol = (MgStringCollection*)NULL;
        if (!message.empty())
        {
            strCol = new MgStringCollection();
            strCol->Add(message);
        }
        throw new MgInvalidFeatureSourceException(L"MgFdoConnectionManager.SetConfiguration",
            __LINE__, __WFILE__, (MgStringCollection*)strCol, L"", NULL);
    }

    Ptr<MgByte> bytes;
    bytes = NULL;

    MgByteSink byteSink(byteReader);
    bytes = byteSink.ToBuffer();

    if(bytes)
    {
        FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
        stream->Write((FdoByte*)bytes->Bytes(), (FdoSize)bytes->GetLength());
        pFdoConnection->SetConfiguration(stream);
    }
}


bool MgFdoConnectionManager::SupportsCommand(FdoIConnection* pFdoConnection, INT32 commandType)
{
    CHECKNULL((FdoIConnection*)pFdoConnection, L"MgFdoConnectionManager.SupportsCommand()");

    FdoPtr<FdoICommandCapabilities> fcc = pFdoConnection->GetCommandCapabilities();
    CHECKNULL((FdoICommandCapabilities*)fcc, L"MgFdoConnectionManager::SupportsCommand");

    bool supports = false;

    // Find all supported command types
    FdoInt32 cnt = 0;
    FdoInt32* fcmd = fcc->GetCommands(cnt);
    if (cnt > 0 && fcmd != NULL)
    {
        for (FdoInt32 i=0; i < cnt; i++)
        {
            if (fcmd[i] == (FdoInt32)commandType)
            {
                supports = true;
            }
        }
    }

    return supports;
}


void MgFdoConnectionManager::ValidateFeatureSource(string& featureSourceXmlContent)
{
    bool isValidFeatureSource = true;

    // TODO: Should we add XML validation here to ensure the integrity of feature source
    if (featureSourceXmlContent.empty())
    {
        isValidFeatureSource = false;
    }
    else
    {
        int index = (int)featureSourceXmlContent.find("<FeatureSource");
        if (index == -1)
        {
            isValidFeatureSource = false;
        }
    }

    // if invalid FeatureSource, throw exception saying invalid provider specified
    if (!isValidFeatureSource)
    {
        STRING message = MgUtil::GetResourceMessage(MgResources::FeatureService, L"MgInvalidFdoProvider");

        Ptr<MgStringCollection> strCol = (MgStringCollection*)NULL;
        if (!message.empty())
        {
            strCol = new MgStringCollection();
            strCol->Add(message);
        }

        throw new MgInvalidFeatureSourceException(L"MgFdoConnectionManager.ValidateFeatureSource",
            __LINE__, __WFILE__, (MgStringCollection*)strCol, L"", NULL);
    }
}


void MgFdoConnectionManager::Open(FdoIConnection* pFdoConnection)
{
    try
    {
        // Open the connection to the FDO provider
        if (pFdoConnection != NULL)
        {
            pFdoConnection->Open();
        }
    }
    catch (FdoException* e)
    {
        STRING messageId;
        MgStringCollection arguments;
        wchar_t* buf = (wchar_t*)e->GetExceptionMessage();

        if (NULL != buf)
        {
            messageId = L"MgFormatInnerExceptionMessage";
            arguments.Add(buf);
        }

        FDO_SAFE_RELEASE(e);

        throw new MgFdoException(L"MgFdoConnectionManager.Open",
            __LINE__, __WFILE__, NULL, messageId, &arguments);
    }
    catch (...)
    {
        throw new MgConnectionFailedException(L"MgFdoConnectionManager.Open",
            __LINE__, __WFILE__, NULL, L"", NULL);
    }
}

MgSpatialContextInfoMap* MgFdoConnectionManager::GetSpatialContextInfo(MgResourceIdentifier* resourceIdentifier)
{
    MgSpatialContextInfoMap* spatialContextInfoMap = NULL;

    MG_FDOCONNECTION_MANAGER_TRY()

    // Retrieve XML from repository
    string featureSourceXmlContent;
    RetrieveFeatureSource(resourceIdentifier, featureSourceXmlContent);

    // Parse XML and get properties

    auto_ptr<MdfModel::FeatureSource> featureSource(GetFeatureSource(resourceIdentifier));

    // Get the supplementary spatial context information which defines the coordinate system
    // for spatial contexts that are missing this information
    spatialContextInfoMap = new MgSpatialContextInfoMap();
    GetSpatialContextInfoFromXml(featureSource.get(), spatialContextInfoMap);

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.GetSpatialContextInfo")

    return spatialContextInfoMap;
}


STRING MgFdoConnectionManager::UpdateProviderName(CREFSTRING providerName)
{
    STRING providerNameNoVersion = providerName;

    // Remove the version from the provider name if it is found
    // ie: OSGeo.SDF.3.0 = OSGeo.SDF
    STRING::size_type index = providerNameNoVersion.find(L".");
    if(STRING::npos != index)
    {
        index++;

        // Found 1st ".", keep looking for second one
        index = providerNameNoVersion.find(L".", index);
        if(STRING::npos != index)
        {
            // Found 2nd "."
            // Update provider name to not include version
            providerNameNoVersion = providerNameNoVersion.substr(0, index);
        }
    }

    return providerNameNoVersion;
}


bool MgFdoConnectionManager::RemoveCachedFdoConnection(CREFSTRING key)
{
    INT32 connections = 0;
    INT32 connectionsRemoved = 0;

    MG_FDOCONNECTION_MANAGER_TRY()

    // Protect the cache
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, false));

    FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

    // We need to search the entire cache because FDO only supports a thread per connection. 
    // Therefore, there could be more then 1 cached connection to the same FDO provider.
    while(m_FdoConnectionCache.end() != iter)
    {
        STRING cacheKey = iter->first;
        if(ACE_OS::strcasecmp(cacheKey.c_str(), key.c_str()) == 0)
        {
            connections++;

            // We have a key match
            FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
            if(pFdoConnectionCacheEntry)
            {
                if(pFdoConnectionCacheEntry->pFdoConnection)
                {
                    INT32 refCount = pFdoConnectionCacheEntry->pFdoConnection->GetRefCount();

                    // We have a match, is it in use?
                    if(1 == refCount)
                    {
                        // Close the connection
                        pFdoConnectionCacheEntry->pFdoConnection->Close();

                        // Release any resource
                        FDO_SAFE_RELEASE(pFdoConnectionCacheEntry->pFdoConnection);

                        delete pFdoConnectionCacheEntry;
                        pFdoConnectionCacheEntry = NULL;

                        // Remove any feature service cache entries for this resource
                        MgServiceManager* serviceManager = MgServiceManager::GetInstance();
                        assert(NULL != serviceManager);

                        try
                        {
                            Ptr<MgResourceIdentifier> resource = new MgResourceIdentifier(key);
                            serviceManager->RemoveFeatureServiceCacheEntry(resource);
                        }
                        catch(MgInvalidRepositoryTypeException* e)
                        {
                            // If this exception is thrown then the key was not a resource identifier string 
                            // and so there will be no entries in the feature service cache to remove.
                            SAFE_RELEASE(e);
                        }

                        m_FdoConnectionCache.erase(iter++);

                        connectionsRemoved++;

                        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager.RemoveCachedFdoConnection() - Releasing cached FDO connection.\n")));
                    }
                    else
                    {
                        // The resource is still in use and so it cannot be removed
                        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager.RemoveCachedFdoConnection() - FDO connection in use!\n")));

                        // Next cached FDO connection
                        iter++;
                    }
                }
                else
                {
                    // NULL pointer
                    break;
                }
            }
            else
            {
                // NULL pointer
                break;
            }
        }
        else
        {
            // Next cached FDO connection
            iter++;
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.RemoveCachedFdoConnection")

    return (connections == connectionsRemoved);
}

void MgFdoConnectionManager::CacheFdoConnection(FdoIConnection* pFdoConnection, CREFSTRING key, string& data, CREFSTRING ltName)
{
    MG_FDOCONNECTION_MANAGER_TRY()

    // Protect the cache
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, ));

    // Add this entry to the cache
    FdoConnectionCacheEntry* pFdoConnectionCacheEntry = new FdoConnectionCacheEntry;
    if(pFdoConnectionCacheEntry)
    {
        pFdoConnectionCacheEntry->data = data;
        pFdoConnectionCacheEntry->ltName = ltName;
        pFdoConnectionCacheEntry->pFdoConnection = pFdoConnection;
        pFdoConnectionCacheEntry->lastUsed = ACE_OS::gettimeofday();

        #ifdef _DEBUG
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("CacheFdoConnection:\nConnection: %@\nKey = %W\nData = %W\nVersion(LT) = %W\n\n"), (void*)pFdoConnection, key.c_str(), data.empty() ? L"(empty)" : L"(data)", ltName.empty() ? L"(empty)" : ltName.c_str()));
        #endif

        m_FdoConnectionCache.insert(FdoConnectionCache_Pair(key, pFdoConnectionCacheEntry));

        // Increase the reference count before returning it because this entry has been cached
        FDO_SAFE_ADDREF(pFdoConnection);
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.CacheFdoConnection")
}


bool MgFdoConnectionManager::FdoConnectionCacheFull(void)
{
    bool bCacheFull = false;

    MG_FDOCONNECTION_MANAGER_TRY()

    // Protect the cache
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex, true));

    if((INT32)m_FdoConnectionCache.size() >= m_nFdoConnectionPoolSize)
    {
        // We are full, but are all entries in use?
        bCacheFull = true;
    }

    // See if we can make room in the FDO connection cache by removing an unused connection
    if(bCacheFull)
    {
        FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

        while(m_FdoConnectionCache.end() != iter)
        {
            FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
            if(pFdoConnectionCacheEntry)
            {
                // Is it in use?
                if(pFdoConnectionCacheEntry->pFdoConnection)
                {
                    INT32 refCount = pFdoConnectionCacheEntry->pFdoConnection->GetRefCount();
                    
                    // Is it in use?
                    if(1 == refCount)
                    {
                        // Close the connection
                        pFdoConnectionCacheEntry->pFdoConnection->Close();

                        // Release any resource
                        FDO_SAFE_RELEASE(pFdoConnectionCacheEntry->pFdoConnection);

                        delete pFdoConnectionCacheEntry;
                        pFdoConnectionCacheEntry = NULL;

                        m_FdoConnectionCache.erase(iter++);

                        bCacheFull = false;
                        break;
                    }
                    else
                    {
                        // Next cached connection
                        iter++;
                    }
                }
                else
                {
                    // NULL pointer
                    break;
                }
            }
            else
            {
                // NULL pointer
                break;
            }
        }
    }

    MG_FDOCONNECTION_MANAGER_CATCH_AND_THROW(L"MgFdoConnectionManager.FdoConnectionCacheFull")

    return bCacheFull;
}


void MgFdoConnectionManager::ClearCache()
{
    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, sm_mutex));

#ifdef _DEBUG
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager::ClearCache() - FDO cache BEFORE\n")));
    ShowCache();
#endif

    size_t cacheSize = m_FdoConnectionCache.size();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager::ClearCache()\n")));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) Releasing %d cached FDO connections.\n"), cacheSize));

    // Cleanup the FDO connection cache
    FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();

    while(m_FdoConnectionCache.end() != iter)
    {
        STRING key = iter->first;
        FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
        if(pFdoConnectionCacheEntry)
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("    %W - "), key.c_str()));

            if(pFdoConnectionCacheEntry->pFdoConnection)
            {
                INT32 refCount = pFdoConnectionCacheEntry->pFdoConnection->GetRefCount();

                if(1 == refCount)
                {
                    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Removed\n")));

                    // The FDO cache is the only one with a reference
                    // Close the connection
                    pFdoConnectionCacheEntry->pFdoConnection->Close();

                    // Release any resource
                    FDO_SAFE_RELEASE(pFdoConnectionCacheEntry->pFdoConnection);

                    delete pFdoConnectionCacheEntry;
                    pFdoConnectionCacheEntry = NULL;

                    m_FdoConnectionCache.erase(iter++);
                }
                else
                {
                    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Still in use!!\n")));
                    // Next cached connection
                    iter++;
                }
            }
            else
            {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Removed (NULL connection)\n")));

                // Remove NULL FDO connection entry
                delete pFdoConnectionCacheEntry;
                pFdoConnectionCacheEntry = NULL;

                m_FdoConnectionCache.erase(iter++);
            }
        }
        else
        {
            // NULL pointer
            break;
        }
    }

#ifdef _DEBUG
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager::ClearCache() - FDO cache AFTER\n")));
    ShowCache();
    cacheSize = m_FdoConnectionCache.size();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) FDO cached connections still in use %d.\n"), cacheSize));
#endif
}


#ifdef _DEBUG
void MgFdoConnectionManager::ShowCache(void)
{
    MG_FDOCONNECTION_MANAGER_TRY()

    size_t cacheSize = m_FdoConnectionCache.size();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) MgFdoConnectionManager::ShowCache()\n")));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) Cached FDO connections = %d\n"), cacheSize));

    // Show the contents of the FDO connection cache
    int nIndex = 1;
    for (FdoConnectionCache::iterator iter = m_FdoConnectionCache.begin();iter != m_FdoConnectionCache.end(); iter++)
    {
        STRING key = iter->first;
        FdoConnectionCacheEntry* pFdoConnectionCacheEntry = iter->second;
        if(pFdoConnectionCacheEntry)
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  %4d) %W\n"), nIndex++, key.c_str()));
        }
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\n")));

    MG_FDOCONNECTION_MANAGER_CATCH(L"MgFdoConnectionManager.ShowCache")
}
#endif


bool MgFdoConnectionManager::IsExcludedProvider(CREFSTRING providerName)
{
    bool bResult = false;

    if(m_excludedProviders.p)
    {
        bResult = m_excludedProviders->Contains(providerName);
    }

    return bResult;
}

MdfModel::FeatureSource* MgFdoConnectionManager::GetFeatureSource(MgResourceIdentifier* resId)
{
    // Retrieve XML from repository
    string featureSourceXmlContent;
    RetrieveFeatureSource(resId, featureSourceXmlContent);

    MdfParser::FSDSAX2Parser parser;
    parser.ParseString(featureSourceXmlContent.c_str(), featureSourceXmlContent.length()*sizeof(char));

    assert(parser.GetSucceeded());

    // detach the feature source from the parser - it's
    // now the caller's responsibility to delete it
    MdfModel::FeatureSource* fs = parser.DetachFeatureSource();

    assert(fs != NULL);
    return fs;
}

