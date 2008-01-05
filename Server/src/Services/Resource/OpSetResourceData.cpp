//
//  Copyright (C) 2004-2007 by Autodesk, Inc.
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

#include "ResourceServiceDefs.h"
#include "OpSetResourceData.h"
#include "ServerResourceService.h"
#include "LogManager.h"


///----------------------------------------------------------------------------
/// <summary>
/// Constructs the object.
/// </summary>
///----------------------------------------------------------------------------
MgOpSetResourceData::MgOpSetResourceData()
{
}


///----------------------------------------------------------------------------
/// <summary>
/// Destructs the object.
/// </summary>
///----------------------------------------------------------------------------
MgOpSetResourceData::~MgOpSetResourceData()
{
}


///----------------------------------------------------------------------------
/// <summary>
/// Executes the operation.
/// </summary>
///
/// <exceptions>
/// MgException
/// </exceptions>
///----------------------------------------------------------------------------
void MgOpSetResourceData::Execute()
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  (%t) MgOpSetResourceData::Execute()\n")));

    MG_LOG_OPERATION_MESSAGE(L"SetResourceData");

    MG_RESOURCE_SERVICE_TRY()

    MG_LOG_OPERATION_MESSAGE_INIT(m_packet.m_OperationVersion, m_packet.m_NumArguments);

    ACE_ASSERT(m_stream != NULL);

    if (4 == m_packet.m_NumArguments)
    {
        STRING dataName;
        STRING dataType;
        Ptr<MgResourceIdentifier> resource = (MgResourceIdentifier*)m_stream->GetObject();
        m_stream->GetString(dataName);
        m_stream->GetString(dataType);
        Ptr<MgByteReader> data = (MgByteReader*)m_stream->GetObject();

        BeginExecution();

        MG_LOG_OPERATION_MESSAGE_PARAMETERS_START();
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"MgResourceIdentifier");
        MG_LOG_OPERATION_MESSAGE_ADD_SEPARATOR();
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(dataName.c_str());
        MG_LOG_OPERATION_MESSAGE_ADD_SEPARATOR();
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(dataType.c_str());
        MG_LOG_OPERATION_MESSAGE_ADD_SEPARATOR();
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"MgByteReader");
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_END();

        Validate();

        m_service->SetResourceData(resource, dataName, dataType, data);

        EndExecution();
    }
    else
    {
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_START();
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_END();
    }

    if (!m_argsRead)
    {
        throw new MgOperationProcessingException(L"MgOpSetResourceData.Execute",
            __LINE__, __WFILE__, NULL, L"", NULL);
    }

    // Successful operation
    MG_LOG_OPERATION_MESSAGE_ADD_STRING(MgResources::Success.c_str());

    MG_RESOURCE_SERVICE_CATCH(L"MgOpSetResourceData.Execute")

    if (mgException != NULL)
    {
        // Failed operation
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(MgResources::Failure.c_str());
    }

    // Add access log entry for operation
    MG_LOG_OPERATION_MESSAGE_ACCESS_ENTRY();

    MG_RESOURCE_SERVICE_THROW()
}
