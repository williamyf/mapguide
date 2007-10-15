/*
 * Copyright (C) 2007 by Autodesk, Inc. All Rights Reserved.
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
#include "ExpressionFunctionFeatureClass.h"


ExpressionFunctionFeatureClass::ExpressionFunctionFeatureClass(const wchar_t* featureClass)
{
    m_featureClassValue = FdoStringValue::Create(featureClass);
    m_functionDefinition = NULL;
}


ExpressionFunctionFeatureClass::~ExpressionFunctionFeatureClass()
{
    m_featureClassValue->Release();
    FDO_SAFE_RELEASE(m_functionDefinition);
}


ExpressionFunctionFeatureClass* ExpressionFunctionFeatureClass::Create(const wchar_t* featureClass)
{
    return new ExpressionFunctionFeatureClass(featureClass);
}


FdoFunctionDefinition* ExpressionFunctionFeatureClass::GetFunctionDefinition()
{
    if (!m_functionDefinition)
    {
        FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();
        m_functionDefinition = FdoFunctionDefinition::Create(L"FEATURECLASS",
                                                             L"Returns the active feature class name",
                                                             FdoDataType_String,
                                                             args,
                                                             FdoFunctionCategoryType_String);
    }

    return FDO_SAFE_ADDREF(m_functionDefinition);
}


FdoLiteralValue* ExpressionFunctionFeatureClass::Evaluate(FdoLiteralValueCollection* literalValues)
{
    // make sure we have zero arguments
    if (literalValues->GetCount() != 0)
        throw FdoExpressionException::Create(L"Incorrect number of arguments for function FEATURECLASS");

    m_featureClassValue->AddRef();
    return m_featureClassValue;
}


FdoExpressionEngineIFunction* ExpressionFunctionFeatureClass::CreateObject()
{
    return new ExpressionFunctionFeatureClass(L"");
}


void ExpressionFunctionFeatureClass::Dispose()
{
    delete this;
}
