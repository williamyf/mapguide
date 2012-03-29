//
//  Copyright (C) 2004-2011 by Autodesk, Inc.
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
#include "LogThread.h"
#include "LogEntryData.h"
#include "ServerManager.h"
#include "LogManager.h"

//////////////////////////////////////////////////////////////////
/// <summary>
/// Constructor
/// </summary>
MgLogThread::MgLogThread(ACE_Thread_Manager &tm, INT32 nThreads) :
    MgThreadBase(tm, nThreads)
{
    m_bActive = true;
}

//////////////////////////////////////////////////////////////////
/// <summary>
/// ACE_Task method
/// </summary>
int MgLogThread::svc()
{
    INT32 nResult = 0;

    Ptr<MgException> mgException;
    try
    {
        MgLogManager* pLogManager = MgLogManager::GetInstance();

        while (m_bActive)
        {
//            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%t) MgLogThread::svc() Ready\n")));

            ACE_Message_Block* messageBlock = NULL;

            nResult = getq(messageBlock);
            if(nResult == -1)
            {
                INT32 nError = ACE_OS::last_error();

                if(nError == EINTR)
                {
                    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  (%t) Interrupted while waiting for message\n")));
                    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  (%t) MgLogThread - Exiting thread\n")));
                    return 0;
                }
                else
                {
                    // There was an error
                    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  (%t) MgLogThread - Exiting thread\n")));
                    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("MgLogThread::svc()")), -1);
                }
            }

            if(messageBlock)
            {
                if(messageBlock->msg_type() == ACE_Message_Block::MB_STOP)
                {
                    m_bActive = false;

                    ACE_Message_Block* mb = new ACE_Message_Block(4);
                    if(mb)
                    {
                        mb->msg_type(ACE_Message_Block::MB_STOP);
                        putq(mb);
                    }
                }
                else if(messageBlock->msg_type() == ACE_Message_Block::MB_DATA)
                {
                    // Get the function
                    MgLogEntryData* led = (MgLogEntryData*)messageBlock->data_block();
                    if(led)
                    {
                        Ptr<MgException> mgException;
                        try
                        {
                            pLogManager->WriteLogMessage(led->m_logType, led->m_message, led->m_logPriority);
                        }
                        catch(MgException* e)
                        {
                            MgServerManager* pServerManager = MgServerManager::GetInstance();

                            ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), e->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
                            MG_LOG_EXCEPTION_ENTRY(e->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), e->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());

                            SAFE_RELEASE(e);
                        }
                        catch(FdoException* e)
                        {
                            STRING messageId;
                            MgStringCollection arguments;
                            wchar_t* buf = (wchar_t*)e->GetExceptionMessage();

                            if (NULL != buf)
                            {
                                messageId = L"MgFormatInnerExceptionMessage";
                                arguments.Add(buf);
                            }

                            MgServerManager* pServerManager = MgServerManager::GetInstance();

                            mgException = new MgFdoException(L"MgLogThread.svc", __LINE__, __WFILE__, NULL, messageId, &arguments);
                            ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), mgException->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
                            MG_LOG_EXCEPTION_ENTRY(mgException->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), mgException->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());

                            FDO_SAFE_RELEASE(e);
                        }
                        catch (exception& e)
                        {
                            MgServerManager* pServerManager = MgServerManager::GetInstance();

                            mgException = MgSystemException::Create(e, L"MgLogThread.svc", __LINE__, __WFILE__);
                            ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), mgException->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
                            MG_LOG_EXCEPTION_ENTRY(mgException->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), mgException->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());
                        }
                        catch(...)
                        {
                            MgServerManager* pServerManager = MgServerManager::GetInstance();

                            mgException = new MgUnclassifiedException(L"MgLogThread.svc", __LINE__, __WFILE__, NULL, L"", NULL);
                            ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), mgException->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
                            MG_LOG_EXCEPTION_ENTRY(mgException->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), mgException->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());
                        }
                    }
                }

                //  Cleanup message block
                messageBlock->release();
                messageBlock = NULL;
            }
        }
    }
    catch (MgException* e)
    {
        MgServerManager* pServerManager = MgServerManager::GetInstance();

        ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), e->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
        MG_LOG_EXCEPTION_ENTRY(e->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), e->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());

        SAFE_RELEASE(e);

        nResult = -1;
    }
    catch (exception& e)
    {
        MgServerManager* pServerManager = MgServerManager::GetInstance();

        mgException = MgSystemException::Create(e, L"MgLogThread.svc", __LINE__, __WFILE__);
        ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), mgException->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
        MG_LOG_EXCEPTION_ENTRY(mgException->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), mgException->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());

        nResult = -1;
    }
    catch (...)
    {
        MgServerManager* pServerManager = MgServerManager::GetInstance();

        mgException = new MgUnclassifiedException(L"MgLogThread.svc", __LINE__, __WFILE__, NULL, L"", NULL);
        ACE_DEBUG ((LM_ERROR, ACE_TEXT("(%t) %W\n"), mgException->GetDetails(pServerManager->GetDefaultMessageLocale()).c_str()));
        MG_LOG_EXCEPTION_ENTRY(mgException->GetExceptionMessage(pServerManager->GetDefaultMessageLocale()).c_str(), mgException->GetStackTrace(pServerManager->GetDefaultMessageLocale()).c_str());

        nResult = -1;
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%t) MgLogThread - Exiting thread\n")));
    return nResult;
}
