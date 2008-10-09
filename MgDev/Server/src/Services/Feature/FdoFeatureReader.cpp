/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */
#include "stdafx.h"
#include <assert.h>
#include "Fdo.h"
#include "FdoCommon.h"
#include "FdoFeatureReader.h"

#include <limits>       // For quiet_NaN()
using namespace std;


// Class MgFdoFeatureReader
MgFdoFeatureReader::MgFdoFeatureReader() : m_currentReaderId(-1)
{
    m_readerCollection = MgFdoReaderCollection::Create();
}

MgFdoFeatureReader::MgFdoFeatureReader(MgFdoReaderCollection *readerCollection) : m_currentReaderId(-1)
{
    m_readerCollection = FDO_SAFE_ADDREF(readerCollection);
}

MgFdoFeatureReader::~MgFdoFeatureReader()
{
    MG_TRY()
    
    Close();

    MG_CATCH_AND_RELEASE()
}

void MgFdoFeatureReader::Dispose()
{
    delete this;
}

FdoClassDefinition *MgFdoFeatureReader::GetClassDefinition()
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetClassDefinition());
}


int MgFdoFeatureReader::GetDepth()
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetDepth();
}



FdoIFeatureReader* MgFdoFeatureReader::GetFeatureObject( const wchar_t* propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetFeatureObject(propertyName));
}

bool MgFdoFeatureReader::GetBoolean( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetBoolean(propertyName);
}

FdoByte MgFdoFeatureReader::GetByte( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetByte(propertyName);
}


FdoDateTime MgFdoFeatureReader::GetDateTime( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetDateTime(propertyName);
}


double MgFdoFeatureReader::GetDouble( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetDouble(propertyName);
}

const wchar_t* MgFdoFeatureReader::GetString( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetString(propertyName);

}

short MgFdoFeatureReader::GetInt16( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetInt16(propertyName);
}

int MgFdoFeatureReader::GetInt32( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetInt32(propertyName);
}

FdoInt64 MgFdoFeatureReader::GetInt64( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetInt64(propertyName);
}

float MgFdoFeatureReader::GetSingle( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetSingle(propertyName);
}

///////////////////////////////////////////////////////////////////////////////
FdoIStreamReader* MgFdoFeatureReader::GetLOBStreamReader(const wchar_t* propertyName)
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetLOBStreamReader(propertyName));
}

///////////////////////////////////////////////////////////////////////////////
FdoLOBValue* MgFdoFeatureReader::GetLOB(const wchar_t* propertyName)
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetLOB(propertyName));
}

bool MgFdoFeatureReader::IsNull( const wchar_t *propertyName )
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->IsNull(propertyName);
}

FdoByteArray* MgFdoFeatureReader::GetGeometry(const wchar_t* propertyName)
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetGeometry(propertyName));
}


const FdoByte * MgFdoFeatureReader::GetGeometry(const wchar_t* propertyName, FdoInt32 * count)
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return m_currentReader->GetGeometry(propertyName, count);
}

/// <summary>Gets the raster object of the specified property.
/// Because no conversion is performed, the property must be
/// of Raster type; otherwise, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param>
/// <returns>Returns the raster object.</returns>
FdoIRaster* MgFdoFeatureReader::GetRaster(FdoString* propertyName)
{
    if (m_currentReader == NULL)
    {
        Initialize();
    }

    return FDO_SAFE_ADDREF(m_currentReader->GetRaster(propertyName));
}


bool MgFdoFeatureReader::ReadNext( )
{
   bool retVal = false;
   if (m_currentReader == NULL)
   {
       Initialize();
   }

   if (m_currentReader->ReadNext())
   {
        retVal = true;
   }
   else
   {
       m_currentReaderId++;
       if (m_readerCollection->GetCount() > m_currentReaderId)
       {
           m_currentReader = (FdoIFeatureReader*)m_readerCollection->GetItem(m_currentReaderId);
           if (m_currentReader == NULL)
           {
               retVal = false;
           }
           else
           {
                retVal = m_currentReader->ReadNext();
           }
       }
   }

   return retVal;

}

void MgFdoFeatureReader::Close( )
{
    if (m_currentReader)
    {
        m_currentReader->Close();
    }
}

void MgFdoFeatureReader::Add(FdoIFeatureReader *reader)
{
    m_readerCollection->Add(reader);
}

void MgFdoFeatureReader::Initialize()
{
    if (m_currentReader == NULL)
    {
        if (m_readerCollection->GetCount())
        {
            m_currentReader = (FdoIFeatureReader*)m_readerCollection->GetItem(0);
            m_currentReaderId = 0;
        }
    }
}
