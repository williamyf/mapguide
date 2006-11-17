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

#include "MapGuideCommon.h"
#include "CryptographyUtil.h"

#undef GetUserName

static ACE_thread_key_t g_threadLocalUserInformation = 0;

IMPLEMENT_DYNCREATE(MgUserInformation);

///////////////////////////////
///<summary>
///Constructs an unintialized user information object
///</summary>
///<returns>
///Nothing
///</returns>
MgUserInformation::MgUserInformation()
{
    m_type = uitNone;
}

///////////////////////////////
///<summary>
///Constructs an information object from a session identifier
///</summary>
MgUserInformation::MgUserInformation(CREFSTRING sessionId)
{
    m_sessionId = sessionId;
    m_type = uitMgSession;
}

///////////////////////////////
///<summary>
///Constructs credential object with a Mg user name and password.
///</summary>
///<param name="userName">
///Mg user name
///</param>
///<param name="password">
///user password
///</param>
///<returns>
///Nothing
///</returns>
MgUserInformation::MgUserInformation(CREFSTRING userName, CREFSTRING password)
{
    m_username = userName;
    m_password = password;
    m_type = uitMg;
}


///////////////////////////////
///<summary>
///Destructor
///</summary>
///<returns>
///Nothing
///</returns>
MgUserInformation::~MgUserInformation()
{
}

///////////////////////////////
///<summary>
///Initializes userInformation with a Mg user name and password.  The
///userInformation type is set to uitMg
///</summary>
///<param name="userName">
///Mg user name
///</param>
///<param name="password">
///user password
///</param>
///<returns>
///Nothing
///</returns>
void MgUserInformation::SetMgUsernamePassword(CREFSTRING userName, CREFSTRING password)
{
    m_username = userName;
    m_password = password;
    m_type = uitMg;
}


///////////////////////////////
///<summary>
///Returns the type of supplied userInformation
///</summary>
///<returns>
///The type of supplied userInformation
///</returns>
 UserInformationType MgUserInformation::GetType() { return m_type; }

///////////////////////////////
///<summary>
/// Returns the username stored
///</summary>
///<returns>
/// Username stored
///</returns>
STRING MgUserInformation::GetUserName() { return m_username; }

///////////////////////////////
///<summary>
/// Returns the password stored
///</summary>
///<returns>
/// password stored
///</returns>
STRING MgUserInformation::GetPassword() { return m_password; }

///////////////////////////////
///<summary>
/// Returns the session identifier stored
///</summary>
STRING MgUserInformation::GetMgSessionId()
{
    return m_sessionId;
}

///////////////////////////////
///<summary>
///Initializes user information with a Mg session identifier.
///</summary>
void MgUserInformation::SetMgSessionId(CREFSTRING sessionId)
{
    int sepChar = (int)sessionId.find(L"_");

    if (sepChar > 0 && sepChar < (int)sessionId.length())
    {
        m_locale = sessionId.substr(sepChar+1);
        m_sessionId = sessionId;
    }
    else
    {
        MgStringCollection arguments;
        arguments.Add(L"1");
        arguments.Add(sessionId);

        throw new MgInvalidArgumentException(L"MgUserInformation.SetMgSessionId",
            __LINE__, __WFILE__, &arguments, L"MgInvalidSessionsId", NULL);
    }
}

///////////////////////////////
///<summary>
///Sets the desired locale for the user.
///Locale codes are 3 letters language codes defined by ISO 639-2. Please refer
///to  http://www.loc.gov/standards/iso639-2/
///</summary>
void MgUserInformation::SetLocale(CREFSTRING locale)
{
    m_locale = locale;
}


///////////////////////////////
///<summary>
///Retrieves the locale for the user.
///</summary>
STRING MgUserInformation::GetLocale()
{
    return m_locale;
}

void MgUserInformation::SetClientAgent(CREFSTRING agent)
{
    m_clientAgent = agent;
}

STRING MgUserInformation::GetClientAgent()
{
    return m_clientAgent;
}

void MgUserInformation::SetClientIp(CREFSTRING ip)
{
    m_clientIp = ip;
}

STRING MgUserInformation::GetClientIp()
{
    return m_clientIp;
}

///////////////////////////////
///<summary>
///Create a sessionid
///</summary>
STRING MgUserInformation::CreateMgSessionId()
{
    STRING uuid;
    MgUtil::GenerateUuid(uuid);

    // TODO: Pull default locale from MgConfiguration within a try/catch
    STRING locale = m_locale.empty() ? MgResources::DefaultLocale : m_locale;

    uuid.append(L"_");
    uuid.append(locale);

    return uuid;
}

///////////////////////////////
///<summary>
///Clears the current sessionid
///</summary>
void MgUserInformation::ClearMgSessionId()
{
    m_sessionId = L"";
}

/// <summary>
/// Self destructor
/// </summary>
/// <returns> Nothing
/// </returns>
void MgUserInformation::Dispose() { delete this; }

/////////////////////////////////////////////////////////////////
/// <summary>
/// Returns the classId.
/// </summary>
INT32 MgUserInformation::GetClassId() { return m_cls_id; }

MgByteReader* MgUserInformation::GetReader()
{
    throw new MgNotImplementedException(L"MgUserInformation.GetReader", __LINE__, __WFILE__, NULL, L"", NULL);
}

///////////////////////////////
///<summary>
///Sets the user information for the current thread.  This
///function uses thread local storage.
///</summary>
void MgUserInformation::SetCurrentUserInfo(MgUserInformation* userInformation)
{
    if (0 == g_threadLocalUserInformation)
    {
        // Perform Double-Checked Locking Optimization.
        ACE_MT (ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance ()));
        if (0 == g_threadLocalUserInformation)
        {
            if (ACE_OS::thr_keycreate(&g_threadLocalUserInformation, NULL) < 0)
            {
                g_threadLocalUserInformation = 0;
            }
        }
    }

    if (0 != g_threadLocalUserInformation)
    {
        MgUserInformation* oldInfo = NULL;
        ACE_OS::thr_getspecific(g_threadLocalUserInformation, (void**) &oldInfo);

        if (ACE_OS::thr_setspecific(g_threadLocalUserInformation, userInformation) >= 0)
        {
            if (NULL != userInformation)
            {
                SAFE_ADDREF(userInformation);
            }
        }

        if (NULL != oldInfo)
        {
            SAFE_RELEASE(oldInfo);
        }
    }
}

///////////////////////////////
///<summary>
///Gets the user information for the current thread which was
///set previously using SetCurrentUserInfo.  This
///function uses thread local storage.
///</summary>
MgUserInformation* MgUserInformation::GetCurrentUserInfo()
{
    MgUserInformation* userInfo = NULL;
    if (0 != g_threadLocalUserInformation)
    {
        ACE_OS::thr_getspecific(g_threadLocalUserInformation, (void**) &userInfo);
    }

    if (NULL == userInfo)
    {
        // No user information should mean that we have opened the connection to the site server yet.
        throw new MgConnectionNotOpenException(L"MgSiteConnection.GetCurrentUserInfo", __LINE__, __WFILE__, NULL, L"", NULL);
    }

    return userInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Serialize data to TCP/IP stream.
/// </summary>

void MgUserInformation::Serialize(MgStream* stream)
{
    STRING credentials;

    if (!m_username.empty() || !m_password.empty())
    {
        MgCryptographyManager cryptoManager;

        credentials = cryptoManager.EncryptCredentials(m_username, m_password);
    }

    stream->WriteInt32(m_type);
    stream->WriteString(credentials);
    stream->WriteString(m_sessionId);
    stream->WriteString(m_locale);
    stream->WriteString(m_clientAgent);
    stream->WriteString(m_clientIp);
}

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Deserialize data from TCP/IP stream.
/// </summary>

void MgUserInformation::Deserialize(MgStream* stream)
{
    INT32 type;
    STRING credentials;

    stream->GetInt32(type);
    stream->GetString(credentials);
    stream->GetString(m_sessionId);
    stream->GetString(m_locale);
    stream->GetString(m_clientAgent);
    stream->GetString(m_clientIp);

    m_type = (UserInformationType) type;

    if (credentials.empty())
    {
        m_username = m_password = L"";
    }
    else
    {
        MG_CRYPTOGRAPHY_TRY()

        MgCryptographyUtil cryptoUtil;
        string username, password;

        cryptoUtil.DecryptCredentials(MgUtil::WideCharToMultiByte(credentials), 
            username, password);

        MgUtil::MultiByteToWideChar(username, m_username);
        MgUtil::MultiByteToWideChar(password, m_password);

        MG_CRYPTOGRAPHY_CATCH_AND_THROW(L"MgUserInformation.Deserialize")
    }
}
