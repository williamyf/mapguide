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

MG_IMPL_DYNCREATE(MgLayout)

// Default Constructor for a MgLayout object
//
MgLayout::MgLayout()
{
}

// Construct a MgLayout object
//
MgLayout::MgLayout(MgResourceIdentifier* layoutDefinition, CREFSTRING title, CREFSTRING unitType) :
    m_title(title), m_unitType(unitType)
{
    m_layout = SAFE_ADDREF(layoutDefinition);
}

// Destructor for MgLayout
MgLayout::~MgLayout()
{
    if (m_layout)
    {
        SAFE_RELEASE(m_layout);
    }
}

// Get the layout to use when generating the EPlot DWF.  If not specified, then the plotted map
// will fill the space within the page margins.
MgResourceIdentifier* MgLayout::GetLayout()
{
    return SAFE_ADDREF(m_layout);
}

// Set the layout to use when generating the EPlot DWF.
void MgLayout::SetLayout(MgResourceIdentifier* plotLayout)
{
    m_layout = plotLayout;
}

// Get the title
STRING MgLayout::GetTitle()
{
    return m_title;
}

// Set the title
void MgLayout::SetTitle(CREFSTRING title)
{
    m_title = title;
}

// Get the layout scale units.  This value determines the units used
// in the scale bar definition.  This value is ignored if a layout is not
// specified.
STRING MgLayout::GetUnitType()
{
    return m_unitType;
}

// Set the layout scale units.
void MgLayout::SetUnitType(CREFSTRING unitType)
{
    m_unitType = unitType;
}

// Serialize data to a stream
//
void MgLayout::Serialize(MgStream* stream)
{
    stream->WriteObject(m_layout);
    stream->WriteString(m_title);
    stream->WriteString(m_unitType);
}

// Deserialize data from a stream
//
void MgLayout::Deserialize(MgStream* stream)
{
    MgStreamReader* streamReader = (MgStreamReader*)stream;

    m_layout = (MgResourceIdentifier*)streamReader->GetObject();
    streamReader->GetString(m_title);
    streamReader->GetString(m_unitType);
}
