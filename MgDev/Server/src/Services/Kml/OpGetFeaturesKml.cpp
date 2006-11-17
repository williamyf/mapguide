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

#include "OpGetFeaturesKml.h"
#include "LogManager.h"


MgOpGetFeaturesKml::MgOpGetFeaturesKml()
{
}


MgOpGetFeaturesKml::~MgOpGetFeaturesKml()
{
}


void MgOpGetFeaturesKml::Execute()
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  (%t) MgOpGetFeaturesKml::Execute()\n")));
    
    MG_LOG_OPERATION_MESSAGE(L"GetFeaturesKml");

    MG_TRY()

    MG_LOG_OPERATION_MESSAGE_INIT(m_packet.m_OperationVersion, m_packet.m_NumArguments);

    ACE_ASSERT(m_stream != NULL);

    if (7 == m_packet.m_NumArguments)
    {
        Ptr<MgLayer> layer = (MgLayer*)m_stream->GetObject();
        Ptr<MgEnvelope> extents = (MgEnvelope*)m_stream->GetObject();
        INT32 width;
        m_stream->GetInt32(width);
        INT32 height;
        m_stream->GetInt32(height);
        double dpi;
        m_stream->GetDouble(dpi);
        INT32 drawOrder;
        m_stream->GetInt32(drawOrder);
        STRING format;
        m_stream->GetString(format);
        BeginExecution();

        MG_LOG_OPERATION_MESSAGE_PARAMETERS_START();
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"MgLayer");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"MgEnvelope");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"INT32");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"INT32");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"double");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(L"INT32");
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(format.c_str());
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_END();

        Validate();

        Ptr<MgByteReader> kml =
            m_service->GetFeaturesKml(layer, extents, width, height, dpi, drawOrder, format);

        EndExecution(kml);
    }
    else
    {
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_START();
        MG_LOG_OPERATION_MESSAGE_PARAMETERS_END();
    }

    if (!m_argsRead)
    {
        throw new MgOperationProcessingException(L"MgOpGetFeaturesKml.Execute",
            __LINE__, __WFILE__, NULL, L"", NULL);
    }

    // Successful operation
    MG_LOG_OPERATION_MESSAGE_ADD_STRING(MgResources::Success.c_str());

    MG_CATCH(L"MgOpGetFeaturesKml.Execute")

    if (mgException != NULL)
    {
        // Failed operation
        MG_LOG_OPERATION_MESSAGE_ADD_STRING(MgResources::Failure.c_str());
    }

    // Add access log entry for operation
    MG_LOG_OPERATION_MESSAGE_ACCESS_ENTRY();

    MG_THROW()
}
