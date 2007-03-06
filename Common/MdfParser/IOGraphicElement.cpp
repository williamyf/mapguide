//
//  Copyright (C) 2007 by Autodesk, Inc.
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
#include "IOGraphicElement.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;
using namespace MDFPARSER_NAMESPACE;

IOGraphicElement::IOGraphicElement(GraphicElement* element)
{
    this->_element = element;
}

IOGraphicElement::~IOGraphicElement()
{
}

void IOGraphicElement::ElementChars(const wchar_t *ch)
{
    IF_ENUM_3(m_currElemName, this->_element, GraphicElement, ResizeControl, ch, ResizeNone, AddToResizeBox, AdjustToResizeBox)
}

void IOGraphicElement::EndElement(const wchar_t *name, HandlerStack *handlerStack)
{
    if (m_startElemName == name)
    {
        this->_element = NULL;
        m_startElemName = L"";
        handlerStack->pop();
        delete this;
    }
}

void IOGraphicElement::Write(MdfStream &fd, GraphicElement* element)
{
    EMIT_ENUM_3(fd, element, GraphicElement, ResizeControl, ResizeNone, AddToResizeBox, AdjustToResizeBox, 1)   // ResizeNone is default
}
