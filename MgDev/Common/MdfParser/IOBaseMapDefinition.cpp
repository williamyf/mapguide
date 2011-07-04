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

#include "stdafx.h"
#include "IOBaseMapDefinition.h"
#include "IOVectorLayerDefinition.h"
#include "IODrawingLayerDefinition.h"
#include "IOBaseMapLayerGroup.h"
#include "DisplayScale.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;
using namespace MDFPARSER_NAMESPACE;


IOBaseMapDefinition::IOBaseMapDefinition(Version& version) : SAX2ElementHandler(version)
{
    this->m_map = NULL;
}


IOBaseMapDefinition::IOBaseMapDefinition(MapDefinition* map, Version& version) : SAX2ElementHandler(version)
{
    this->m_map = map;
}


IOBaseMapDefinition::~IOBaseMapDefinition()
{
}


void IOBaseMapDefinition::StartElement(const wchar_t* name, HandlerStack* handlerStack)
{
    this->m_currElemName = name;
    if (this->m_currElemName == L"BaseMapDefinition") // NOXLATE
    {
        this->m_startElemName = name;
    }
    else
    {
        if (this->m_currElemName == L"FiniteDisplayScale")
        {
            // default processing in ElementChars
        }
        else if (this->m_currElemName == L"BaseMapLayerGroup")
        {
            IOBaseMapLayerGroup* IO = new IOBaseMapLayerGroup(this->m_map, this->m_version);
            handlerStack->push(IO);
            IO->StartElement(name, handlerStack);
        }
    }
}


void IOBaseMapDefinition::ElementChars(const wchar_t* ch)
{
    if (this->m_currElemName == L"FiniteDisplayScale") // NOXLATE
    {
        double val = wstrToDouble(ch);
        this->m_map->GetFiniteDisplayScales()->Adopt(new DisplayScale(val));
    }
}


void IOBaseMapDefinition::EndElement(const wchar_t* name, HandlerStack* handlerStack)
{
    if (this->m_startElemName == name)
    {
        this->m_map = NULL;
        this->m_startElemName = L"";
        handlerStack->pop();
        delete this;
    }
}


void IOBaseMapDefinition::Write(MdfStream& fd, MapDefinition* map, Version* version, MgTab& tab)
{
    fd << tab.tab() << "<BaseMapDefinition>" << std::endl; // NOXLATE
    tab.inctab();

    // Property: FiniteDisplayScales
    DisplayScaleCollection* finiteDisplayScales = map->GetFiniteDisplayScales();
    for (int i=0; i<finiteDisplayScales->GetCount(); ++i)
    {
        fd << tab.tab() << "<FiniteDisplayScale>"; // NOXLATE
        fd << DoubleToStr((static_cast<DisplayScale*>(finiteDisplayScales->GetAt(i)))->GetValue());
        fd << "</FiniteDisplayScale>" << std::endl; // NOXLATE
    }

    // Property: BaseMapLayerGroups
    BaseMapLayerGroupCollection* baseMapGroups = map->GetBaseMapLayerGroups();
    for (int i=0; i<baseMapGroups->GetCount(); ++i)
        IOBaseMapLayerGroup::Write(fd, static_cast<BaseMapLayerGroup*>(baseMapGroups->GetAt(i)), version, tab);

    tab.dectab();
    fd << tab.tab() << "</BaseMapDefinition>" << std::endl; // NOXLATE
}