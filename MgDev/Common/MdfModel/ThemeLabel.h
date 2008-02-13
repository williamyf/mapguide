//
//  Copyright (C) 2007-2008 by Autodesk, Inc.
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

#ifndef THEMELABEL_H_
#define THEMELABEL_H_

#include "MdfModel.h"
#include "MdfRootObject.h"

BEGIN_NAMESPACE_MDFMODEL

    //-------------------------------------------------------------------------
    // DESCRIPTION:
    //-------------------------------------------------------------------------
    class MDFMODEL_API ThemeLabel : public MdfRootObject
    {
    public:
        // Construction, initialization
        ThemeLabel();

        const MdfString& GetDescription() const;
        void SetDescription(const MdfString& description);

        const MdfString& GetCategoryFormat() const;
        void SetCategoryFormat(const MdfString& categoryFormat);

    private:
        // Hidden copy constructor and assignment operator.
        ThemeLabel(const ThemeLabel&);
        ThemeLabel& operator=(const ThemeLabel&);

        // Data members
        MdfString m_sDescription;
        MdfString m_sCategoryFormat;
    };

END_NAMESPACE_MDFMODEL
#endif // THEMELABEL_H_
