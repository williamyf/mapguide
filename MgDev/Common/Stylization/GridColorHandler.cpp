//
//  Copyright (C) 2004-2010 by Autodesk, Inc.
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
#include "GridColorHandler.h"
#include "GridColorBandHandler.h"
#include "GridColorBandsHandler.h"
#include "GridColorThemeHandler.h"
#include "GridColorNullHandler.h"

//
// GridColorHandler
//
GridColorHandler::GridColorHandler()
{
}

GridColorHandler::~GridColorHandler()
{
}

GridColorHandler* GridColorHandler::Create(const MdfModel::RuleCollection *pRules, const GridData *pGrid)
{
    GridColorHandler* pHandler = CreateBandHandler(pRules, pGrid);
    if (pHandler)
        return pHandler;

    pHandler = CreateBandsHandler(pRules, pGrid);
    if (pHandler)
        return pHandler;

    pHandler = CreateThemeHandler(pRules, pGrid);
    if (pHandler)
        return pHandler;

    return CreateNullHandler(pRules, pGrid);
}

GridColorBandHandler* GridColorHandler::CreateBandHandler(const MdfModel::RuleCollection *pRules, const GridData *pGrid)
{
    std::auto_ptr<GridColorBandHandler> spHandler(new GridColorBandHandler());
    if (!spHandler->Initialize(pRules, pGrid))
        spHandler.reset();
    return spHandler.release();
}

GridColorBandsHandler* GridColorHandler::CreateBandsHandler(const MdfModel::RuleCollection *pRules, const GridData *pGrid)
{
    std::auto_ptr<GridColorBandsHandler> spHandler(new GridColorBandsHandler());
    if (!spHandler->Initialize(pRules, pGrid))
        spHandler.reset();
    return spHandler.release();
}

GridColorThemeHandler* GridColorHandler::CreateThemeHandler(const MdfModel::RuleCollection *pRules, const GridData *pGrid)
{
    std::auto_ptr<GridColorThemeHandler> spHandler(new GridColorThemeHandler());
    if (!spHandler->Initialize(pRules, pGrid))
        spHandler.reset();
    return spHandler.release();
}

GridColorNullHandler* GridColorHandler::CreateNullHandler(const MdfModel::RuleCollection *pRules, const GridData *pGrid)
{
    std::auto_ptr<GridColorNullHandler> spHandler(new GridColorNullHandler);
    if (!spHandler->Initialize(pRules, pGrid))
        spHandler.reset();
    return spHandler.release();
}
