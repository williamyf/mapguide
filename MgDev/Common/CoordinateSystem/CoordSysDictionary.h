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

#ifndef _CCOORDINATESYSTEMDICTIONARY_H_
#define _CCOORDINATESYSTEMDICTIONARY_H_

namespace CSLibrary
{

class CCoordinateSystemDictionary : public MgCoordinateSystemDictionary
{
public:
    CCoordinateSystemDictionary(MgCoordinateSystemCatalog *pCatalog);
    virtual ~CCoordinateSystemDictionary();

    virtual MgCoordinateSystem* NewCoordinateSystem();

    virtual MgCoordinateSystemCatalog* GetCatalog();
    virtual void SetPath(CREFSTRING sPath);
    virtual STRING GetPath();
    virtual UINT32 GetSize();
    virtual void Add(MgGuardDisposable *pDefinition);
    virtual void Remove(CREFSTRING sName);
    virtual void Modify(MgGuardDisposable *pDefinition);
    virtual MgGuardDisposable* Get(CREFSTRING sName);
    virtual MgCoordinateSystem* GetCoordinateSystem(CREFSTRING sName);
    virtual bool Has(CREFSTRING sName);
    virtual MgCoordinateSystemEnum* GetEnum();

protected:
    //MgDisposable
    virtual void Dispose();

protected:
    //Data members
    STRING m_sPath;
    CSystemNameDescriptionMap *m_pmapSystemNameDescription;
    INT32 m_lMagic;
    MgCoordinateSystemCatalog* m_pCatalog;

private:
    //Unimplemented stuff
    CCoordinateSystemDictionary();
    CCoordinateSystemDictionary(const CCoordinateSystemDictionary&);
    CCoordinateSystemDictionary& operator=(const CCoordinateSystemDictionary&);
};

} // End of namespace

#endif //_CCOORDINATESYSTEMDICTIONARY_H_
