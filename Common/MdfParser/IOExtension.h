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

#ifndef _IOEXTENSION_H
#define _IOEXTENSION_H

#include "../MdfModel/Extension.h"
#include "../MdfModel/FeatureSource.h"
#include "SAX2ElementHandler.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;

BEGIN_NAMESPACE_MDFPARSER

class MDFPARSER_API IOExtension : public SAX2ElementHandler
{
    private:
        Extension *m_pExtension;
        FeatureSource *m_pFeatureSource;

    public:
        IOExtension();
        IOExtension(FeatureSource *pFeatureSource);
        ~IOExtension();
        void Write(MdfStream &fd, Extension *pExtension);

        virtual void StartElement(const wchar_t *name, HandlerStack *handlerStack);
        virtual void ElementChars(const wchar_t *ch);
        virtual void EndElement(const wchar_t *name, HandlerStack *handlerStack);
};

END_NAMESPACE_MDFPARSER
#endif
