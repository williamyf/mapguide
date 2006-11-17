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

/////////////////////////////////////////////////////////////////////
//
// Includes
//
/////////////////////////////////////////////////////////////////////
#include <stdafx.h>

#include "GwsQueryEngineImp.h"



class GwsQueryXmlSaxHandler: public FdoXmlSaxHandler
{
public:

    std::vector<IGWSQueryDefinition *>    m_qdefs;
    WSTR                                  m_elemname;

    GwsQueryXmlSaxHandler (FdoString * elementName)
    {
        m_elemname = elementName;
    }

    virtual ~GwsQueryXmlSaxHandler ()
    {
        for (size_t i = 0; i < m_qdefs.size (); i ++) {
            m_qdefs[i]->Release ();
        }
        m_qdefs.clear ();
    }

    FdoXmlSaxHandler * XmlStartElement(
        FdoXmlSaxContext         * ctx,
        FdoString                * uri,
        FdoString                * name,
        FdoString                * qname,
        FdoXmlAttributeCollection* attrs
     )
    {
        if (! _wcsicmp (name, m_elemname.c_str ())) {
            IGWSQueryDefinition * qdef =
                GwsQueryDefinitionXmlHelpers::ReadQueryDefinition (attrs);
            m_qdefs.push_back (qdef);
        }
        return FdoXmlSaxHandler::XmlStartElement (ctx, uri, name, qname, attrs);
    }
};



/////////////////////////////////////////////////////////////////////
//
// class GWSQueryDefinition
//
/////////////////////////////////////////////////////////////////////
template<class T>
GWSQueryDefinition<T>::GWSQueryDefinition ()
{
}


template<class T>
GWSQueryDefinition<T>::~GWSQueryDefinition () throw()
{
}

template<class T>
void GWSQueryDefinition<T>::ToXmlWriter (
    FdoString    * elementName,
    FdoXmlWriter * writer
)
{
    GwsQueryDefinitionXmlHelpers::WriteQueryDefinitionWithHeader (this, elementName, writer);
}


EGwsStatus GwsQueryDefinitionXmlHelpers::WriteQueryDefinitionWithHeader (
    IGWSQueryDefinition * qdef,
    FdoString           * elementName,
    FdoXmlWriter        * writer
)
{
    if (elementName == NULL || * elementName == 0)
        writer->WriteStartElement (GwsQueryXml::xmlGwsQuery);
    else
        writer->WriteStartElement (elementName);

    CGwsObject * gwsobj = NULL;
    if (qdef != NULL) {
        switch (qdef->Type ()) {
        case eGwsQueryFeature:
            {
                gwsobj = dynamic_cast<CGwsObject*> (qdef);
                writer->WriteAttribute (GwsQueryXml::xmlGwsQueryTypeAttribute, GwsQueryXml::xmlGwsQueryTypeFeature);
            }
            break;
        case eGwsQueryLeftOuterJoin:
            {
                gwsobj = dynamic_cast<CGwsObject*> (qdef);
                writer->WriteAttribute (GwsQueryXml::xmlGwsQueryTypeAttribute, GwsQueryXml::xmlGwsQueryTypeLeftOuterJoin);
            }
            break;
        case eGwsQueryEqualJoin:
            {
                gwsobj = dynamic_cast<CGwsObject*> (qdef);
                writer->WriteAttribute (GwsQueryXml::xmlGwsQueryTypeAttribute, GwsQueryXml::xmlGwsQueryTypeEqualJoin);
            }
            break;
        }
        if (gwsobj != NULL) {
            gwsobj->Write (writer);
        }
    }
    writer->WriteEndElement ();
    if (gwsobj == NULL && qdef != NULL) {
        assert (false);
        return eGwsFailed;
    }
    return eGwsOk;
}

IGWSQueryDefinition * GwsQueryDefinitionXmlHelpers::ReadQueryDefinition (
    FdoXmlAttributeCollection* attrs
)
{

    IGWSQueryDefinition * qrydef = NULL;
    for (FdoInt32 i = 0; i < attrs->GetCount (); i ++) {

        FdoPtr<FdoXmlAttribute> attr = attrs->GetItem (i);

        FdoString * name  = attr->GetName ();
        FdoString * value = attr->GetValue ();
        if (_wcsicmp (name, GwsQueryXml::xmlGwsQueryTypeAttribute) == 0) {
            if (_wcsicmp (value, GwsQueryXml::xmlGwsQueryTypeFeature) == 0) {
                qrydef = new GWSFeatureQueryDefinition ();
                qrydef->AddRef ();
            } else if (_wcsicmp (value, GwsQueryXml::xmlGwsQueryTypeLeftOuterJoin) == 0) {
                qrydef = new GWSLeftJoinQueryDefinition ();
                qrydef->AddRef ();
            } else if (_wcsicmp (value, GwsQueryXml::xmlGwsQueryTypeEqualJoin) == 0) {
                qrydef = new GWSEqualJoinQueryDefinition ();
                qrydef->AddRef ();
            }
        }

    }
    return qrydef;
}

FdoString * GwsQueryDefinitionXmlHelpers::QueryXmlHeader ()
{
    return GwsQueryXml::xmlGwsQuery;
}


IGWSQueryDefinition * IGWSQueryDefinition::FromXmlReader (
    FdoString    * elementName,
    FdoXmlReader * reader
)
{
    FdoPtr<FdoXmlSaxContext> ctx = FdoXmlSaxContext::Create (reader);
    GwsQueryXmlSaxHandler handler (elementName);
    reader->Parse (& handler, ctx, true);

    IGWSQueryDefinition * qdef = NULL;

    if (! handler.m_qdefs.empty ()) {
        // get the first one.
        qdef = handler.m_qdefs [0];
    }
    if (qdef != NULL)
        qdef->AddRef ();
    return qdef;
}


IGWSQueryDefinition * IGWSQueryDefinition::FromXmlAttributes (
    FdoXmlAttributeCollection* attrs
)
{
    return GwsQueryDefinitionXmlHelpers::ReadQueryDefinition (attrs);
}


template class GWSQueryDefinition<IGWSEqualJoinQueryDefinition>;
template class GWSQueryDefinition<IGWSLeftJoinQueryDefinition>;
template class GWSQueryDefinition<IGWSFeatureQueryDefinition>;
