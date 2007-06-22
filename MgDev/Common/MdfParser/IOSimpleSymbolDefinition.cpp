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
#include "IOSimpleSymbolDefinition.h"
#include "IOGraphicElementCollection.h"
#include "IOResizeBox.h"
#include "IOPointUsage.h"
#include "IOLineUsage.h"
#include "IOAreaUsage.h"
#include "IOParameterCollection.h"
#include "IOUnknown.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;
using namespace MDFPARSER_NAMESPACE;


IOSimpleSymbolDefinition::IOSimpleSymbolDefinition(SimpleSymbolDefinition* symbolDefinition)
{
    this->m_symbolDefinition = symbolDefinition;
}


void IOSimpleSymbolDefinition::StartElement(const wchar_t* name, HandlerStack* handlerStack)
{
    this->m_currElemName = name;
    if (this->m_currElemName == L"SimpleSymbolDefinition") // NOXLATE
    {
        this->m_startElemName = name;
    }
    else if (this->m_currElemName == L"Graphics") // NOXLATE
    {
        IOGraphicElementCollection* IO = new IOGraphicElementCollection(this->m_symbolDefinition->GetGraphics());
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"ResizeBox") // NOXLATE
    {
        IOResizeBox* IO = new IOResizeBox(this->m_symbolDefinition);
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"PointUsage") // NOXLATE
    {
        IOPointUsage* IO = new IOPointUsage(this->m_symbolDefinition);
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"LineUsage") // NOXLATE
    {
        IOLineUsage* IO = new IOLineUsage(this->m_symbolDefinition);
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"AreaUsage") // NOXLATE
    {
        IOAreaUsage* IO = new IOAreaUsage(this->m_symbolDefinition);
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"ParameterDefinition") // NOXLATE
    {
        IOParameterCollection* IO = new IOParameterCollection(this->m_symbolDefinition->GetParameterDefinition());
        handlerStack->push(IO);
        IO->StartElement(name, handlerStack);
    }
    else if (this->m_currElemName == L"ExtendedData1") // NOXLATE
    {
        this->m_procExtData = true;
    }
    else
    {
        ParseUnknownXml(name, handlerStack);
    }
}


void IOSimpleSymbolDefinition::ElementChars(const wchar_t* ch)
{
         IF_STRING_PROPERTY(this->m_currElemName, this->m_symbolDefinition, Name, ch)
    else IF_STRING_PROPERTY(this->m_currElemName, this->m_symbolDefinition, Description, ch)
}


void IOSimpleSymbolDefinition::EndElement(const wchar_t* name, HandlerStack* handlerStack)
{
    if (this->m_startElemName == name)
    {
        this->m_symbolDefinition->SetUnknownXml(this->m_unknownXml);

        this->m_symbolDefinition = NULL;
        this->m_startElemName = L"";
        handlerStack->pop();
        delete this;
    }
    else if (::wcscmp(name, L"ExtendedData1") == 0) // NOXLATE
    {
        this->m_procExtData = false;
    }
}


void IOSimpleSymbolDefinition::Write(MdfStream& fd, SimpleSymbolDefinition* symbolDefinition, bool writeAsRootElement, Version* version)
{
    if (writeAsRootElement)
    {
        // we currently only support symbol definition version 1.0.0
        if (version && (*version != Version(1, 0, 0)))
        {
            // TODO - need a way to return error information
            _ASSERT(false);
            return;
        }

        fd << tab() << "<SimpleSymbolDefinition xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"SymbolDefinition-1.0.0.xsd\" version=\"1.0.0\">" << std::endl; // NOXLATE
    }
    else
        fd << tab() << "<SimpleSymbolDefinition>" << std::endl; // NOXLATE
    inctab();

    EMIT_STRING_PROPERTY(fd, symbolDefinition, Name, false, NULL)
    EMIT_STRING_PROPERTY(fd, symbolDefinition, Description, true, L"") // default is empty string

    IOGraphicElementCollection::Write(fd, symbolDefinition->GetGraphics(), version);

    if (symbolDefinition->GetResizeBox())
        IOResizeBox::Write(fd, symbolDefinition->GetResizeBox(), version);

    if (symbolDefinition->GetPointUsage())
        IOPointUsage::Write(fd, symbolDefinition->GetPointUsage(), version);

    if (symbolDefinition->GetLineUsage())
        IOLineUsage::Write(fd, symbolDefinition->GetLineUsage(), version);

    if (symbolDefinition->GetAreaUsage())
        IOAreaUsage::Write(fd, symbolDefinition->GetAreaUsage(), version);

    IOParameterCollection::Write(fd, symbolDefinition->GetParameterDefinition(), version);

    // Write any unknown XML / extended data
    IOUnknown::Write(fd, symbolDefinition->GetUnknownXml(), version);

    dectab();
    fd << tab() << "</SimpleSymbolDefinition>" << std::endl; // NOXLATE
}
