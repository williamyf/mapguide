//
//  Copyright (C) 2004-2009 by Autodesk, Inc.
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

#ifndef _SAX2PARSER_H
#define _SAX2PARSER_H

#include "MapDefinition.h"
#include "VectorLayerDefinition.h"
#include "DrawingLayerDefinition.h"
#include "GridLayerDefinition.h"
#include "SimpleSymbolDefinition.h"
#include "CompoundSymbolDefinition.h"

// disable C4244 of XSerializeEngine about __w64 int assigned to unsigned long
// disable C4267 of XmlScanner about size_t assigned to unsigned int
#pragma warning(push)
#pragma warning(disable: 4244 4267)
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#pragma warning(pop)

#include "IOUtil.h"
#include "SAX2ElementHandler.h"
#include "Version.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;

BEGIN_NAMESPACE_MDFPARSER

// SAX2Parser is the main class for the parser. It holds the actual
// Xerces SAX2 Parser and is also the handler for the events generated
// by the parser. It holds the main references to the map, feature layer,
// or drawing layer that will be created by the parse. It handles both
// reading and writing through public functions.
class MDFPARSER_API SAX2Parser : public DefaultHandler
{
    public:
        SAX2Parser();
        ~SAX2Parser();
        void Flush();
        void Initialize();
        void DisableTabs();
        void ParseFile(std::string str);
        void ParseFile(char* str);
        void ParseString(const char* str, size_t numBytes);
        void ParseString(const wchar_t* str, size_t numChars);
        void WriteToFile(std::string name,
                         MapDefinition* map,
                         VectorLayerDefinition* vLayer,
                         DrawingLayerDefinition* dLayer,
                         GridLayerDefinition* gLayer,
                         MdfModel::Version* version);
        void WriteToFile(std::string name, SymbolDefinition* symbol, MdfModel::Version* version);

        std::string SerializeToXML(MapDefinition* map, MdfModel::Version* version);
        std::string SerializeToXML(LayerDefinition* layer, MdfModel::Version* version);
        std::string SerializeToXML(SymbolDefinition* symbol, MdfModel::Version* version);

        // Detaches the most recently parsed object from the parser.
        // The calling method is then responsible for disposing the object,
        // and the parser no longer has access to this object (set to NULL).
        MapDefinition* DetachMapDefinition();
        VectorLayerDefinition* DetachVectorLayerDefinition();
        DrawingLayerDefinition* DetachDrawingLayerDefinition();
        GridLayerDefinition* DetachGridLayerDefinition();
        LayerDefinition* DetachLayerDefinition();
        SimpleSymbolDefinition* DetachSimpleSymbolDefinition();
        CompoundSymbolDefinition* DetachCompoundSymbolDefinition();
        SymbolDefinition* DetachSymbolDefinition();

        // Creates a clone of the given map/layer/symbol definition.  The
        // object is serialized and parsed into a new object, which is returned.
        static MapDefinition* CreateClone(MapDefinition* map);
        static LayerDefinition* CreateClone(LayerDefinition* layer);
        static SymbolDefinition* CreateClone(SymbolDefinition* symbol);

        bool GetSucceeded() const;

        // Occurs when an XML start tag is encountered. 'localname' is the text in
        // the tag to be paid attention to in our schema.
        void startElement(const XMLCh* const uri,
                          const XMLCh* const localname,
                          const XMLCh* const qname,
                          const Attributes&  attributes);

        // Occurs when an XML closing tag is encountered. 'localname' is the text in
        // the tag to be paid attention to in our schema.
        void endElement(const XMLCh* const uri,
                        const XMLCh* const localname,
                        const XMLCh* const qname);

        // Occurs when characters in between XML tags are encountered.
        void characters(const XMLCh* const chars, const unsigned int length);

        // get error info
        const MdfString& GetErrorMessage();

        // get version info - this only returns meaningful information after parsing
        // a file or string
        const MdfModel::Version& GetVersion();

    protected:
        // error handling override
        virtual void error(const SAXException& exc);

    private:
        // helper methods to extract version info
        void SetLayerDefinitionVersion(const Attributes& attributes);
        void SetSymbolDefinitionVersion(const Attributes& attributes);

    protected:
        // The objects to be generated by the parser.
        MapDefinition* m_map;
        VectorLayerDefinition* m_vLayer;
        DrawingLayerDefinition* m_dLayer;
        GridLayerDefinition* m_gLayer;
        SimpleSymbolDefinition* m_sSymbol;
        CompoundSymbolDefinition* m_cSymbol;

        // Succeeded is true if the parse has succeeded. As of now,
        // there are very loose constraints on this boolean.
        bool m_succeeded;

        SAX2XMLReader* m_parser;

        // handlerStack is a stack of SAX2ElementHandlers (see the
        // typedef in SAX2ElementHandler.h). When the parser receives
        // events they will be routed to the SAX2ElementHandler on the top
        // of the handlerStack, which will be an instance of one of the IO classes,
        // who all implement the SAX2ElementHandler interface.
        HandlerStack* m_handlerStack;

        // Unfortunately, the SAX2 Parser doesn't necessarily send all the
        // characters that appear inside a tag in one call of characters()-
        // for example with escape characters it separates them into separate calls.
        // Therefore, m_strbuffer is used to accumulate the characters.  tagOpen is
        // used in the case when you have characters outside a tag (i.e. <Metadata></Metadata>)
        // to make sure that whitespace or line returns don't get assigned to it instead.
        std::wstring m_strbuffer;
        bool m_tagOpen;

        MdfString m_strParserError;

        MdfModel::Version m_version;
};

END_NAMESPACE_MDFPARSER
#endif // _SAX2PARSER_H
