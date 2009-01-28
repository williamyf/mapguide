/*
 * Copyright (C) 2007-2009 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 */

#include "stdafx.h"
#include "ExpressionFunctionArgb.h"
#include "ExpressionHelper.h"
#include "Foundation.h"


ExpressionFunctionArgb::ExpressionFunctionArgb()
{
    m_argbValue = FdoInt32Value::Create();
    m_functionDefinition = NULL;
}


ExpressionFunctionArgb::~ExpressionFunctionArgb()
{
    FDO_SAFE_RELEASE(m_argbValue);
    FDO_SAFE_RELEASE(m_functionDefinition);
}


ExpressionFunctionArgb* ExpressionFunctionArgb::Create()
{
    return new ExpressionFunctionArgb();
}


FdoFunctionDefinition* ExpressionFunctionArgb::GetFunctionDefinition()
{
    if (!m_functionDefinition)
    {
        STRING funcDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionARGB_Description");
        STRING aValDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionARGB_AValueDescription");
        STRING rValDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionARGB_RValueDescription");
        STRING gValDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionARGB_GValueDescription");
        STRING bValDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionARGB_BValueDescription");

        FdoPtr<FdoArgumentDefinition> arg1 = FdoArgumentDefinition::Create(L"aValue", aValDesc.c_str(), FdoDataType_Int32); // NOXLATE
        FdoPtr<FdoArgumentDefinition> arg2 = FdoArgumentDefinition::Create(L"rValue", rValDesc.c_str(), FdoDataType_Int32); // NOXLATE
        FdoPtr<FdoArgumentDefinition> arg3 = FdoArgumentDefinition::Create(L"gValue", gValDesc.c_str(), FdoDataType_Int32); // NOXLATE
        FdoPtr<FdoArgumentDefinition> arg4 = FdoArgumentDefinition::Create(L"bValue", bValDesc.c_str(), FdoDataType_Int32); // NOXLATE

        FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();
        args->Add(arg1);
        args->Add(arg2);
        args->Add(arg3);
        args->Add(arg4);

        m_functionDefinition = FdoFunctionDefinition::Create(L"ARGB", // NOXLATE
                                                             funcDesc.c_str(),
                                                             FdoDataType_Int32,
                                                             args,
                                                             FdoFunctionCategoryType_String);
    }

    return FDO_SAFE_ADDREF(m_functionDefinition);
}


FdoLiteralValue* ExpressionFunctionArgb::Evaluate(FdoLiteralValueCollection* literalValues)
{
    // make sure we have four arguments
    if (literalValues->GetCount() != 4)
    {
        MgResources* resources = MgResources::GetInstance();
        assert(NULL != resources);

        STRING message = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgIncorrectNumberOfArguments");
        MgStringCollection arguments;
        arguments.Add(L"ARGB"); // NOXLATE
        message = resources->FormatMessage(message, &arguments);

        throw FdoExpressionException::Create(message.c_str());
    }

    FdoPtr<FdoLiteralValue> arg1 = literalValues->GetItem(0);
    FdoPtr<FdoLiteralValue> arg2 = literalValues->GetItem(1);
    FdoPtr<FdoLiteralValue> arg3 = literalValues->GetItem(2);
    FdoPtr<FdoLiteralValue> arg4 = literalValues->GetItem(3);

    FdoInt32 alpha = ExpressionHelper::GetAsInt32(arg1);
    FdoInt32   red = ExpressionHelper::GetAsInt32(arg2);
    FdoInt32 green = ExpressionHelper::GetAsInt32(arg3);
    FdoInt32  blue = ExpressionHelper::GetAsInt32(arg4);

    // clean up
    red   &= 0xFF;
    green &= 0xFF;
    blue  &= 0xFF;
    alpha &= 0xFF;

    FdoInt32 color = (alpha << 24) | (red << 16) | (green << 8) | blue;
    m_argbValue->SetInt32(color);

    return FDO_SAFE_ADDREF(m_argbValue);
}


FdoExpressionEngineIFunction* ExpressionFunctionArgb::CreateObject()
{
    return ExpressionFunctionArgb::Create();
}


void ExpressionFunctionArgb::Dispose()
{
    delete this;
}
