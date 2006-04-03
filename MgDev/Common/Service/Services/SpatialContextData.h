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

#ifndef _MG_SPATIAL_CONTEXT_DATA_H
#define _MG_SPATIAL_CONTEXT_DATA_H

/// \ingroup NOT_EXPOSED_module

class MgStream;

class MgSpatialContextData;
template class Ptr<MgSpatialContextData>;

// INTERNAL_CLASS
class MG_SERVICE_API MgSpatialContextData : public MgNamedSerializable
{
    DECLARE_CREATE_OBJECT()
    DECLARE_CLASSNAME(MgSpatialContextData)

public:

    MgSpatialContextData();
    ~MgSpatialContextData();

    STRING GetName();
    STRING GetDescription();
    STRING GetCoordinateSystem();
    STRING GetCoordinateSystemWkt();
    INT32 GetExtentType();
    MgByteReader* GetExtent();
    double GetXYTolerance();
    double GetZTolerance();
    bool IsActive();

    void SetName(CREFSTRING name);
    void SetDescription(CREFSTRING desc);
    void SetCoordinateSystem(CREFSTRING coordSystem);
    void SetExtentType(INT32 extentType);
    void SetExtent(MgByteReader* byteReader);
    void SetXYTolerance(double xyTol);
    void SetZTolerance(double zTol);
    void SetActiveStatus(bool active);
    void SetCoordinateSystemWkt(CREFSTRING wktString);

    void Serialize(MgStream* stream);
    void Deserialize(MgStream* stream);
    void ToXml(string& xmlStr);
    bool CanSetName();

protected:

    virtual void Dispose()
    {
        delete this;
    }

private:

    STRING m_name;
    STRING m_desc;
    STRING m_coord;
    STRING m_wktStr;
    INT32  m_extent;
    Ptr<MgByteReader> m_byteReader;
    double m_xyTolerance;
    double m_zTolerance;
    bool m_isActive;

INTERNAL_API:

    virtual INT32 GetClassId()
    {
        return m_cls_id;
    }

CLASS_ID:
    static const INT32 m_cls_id = FeatureService_SpatialContextData;
};

#endif
