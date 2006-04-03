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

#ifndef SYMBOLIZATION_H_
#define SYMBOLIZATION_H_

#include "MdfModel.h"

BEGIN_NAMESPACE_MDFMODEL

    class ISymbolizationVisitor;

    //---------------------------------------------------------------------
    // DESCRIPTION:
    // Class Fill is an abstract base that defines how to color/paint
    // feature geometry.
    //---------------------------------------------------------------------
    class MDFMODEL_API Symbolization
    {
    public:
        // Construction, destruction, initialization
        Symbolization();
        virtual ~Symbolization();

        // Operations

        // Visitor Pattern method to be implemented by all concrete
        // subclasses.
        virtual void AcceptVisitor(ISymbolizationVisitor& isyVisitor) = 0;

    private:
        // Data members
    };

END_NAMESPACE_MDFMODEL
#endif //SYMBOLIZATION_H_
