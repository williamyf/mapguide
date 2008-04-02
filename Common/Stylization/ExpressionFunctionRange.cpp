/*
 * Copyright (C) 2007-2008 by Autodesk, Inc. All Rights Reserved.
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
#include "ExpressionFunctionRange.h"
#include "ExpressionHelper.h"
#include "Foundation.h"

// until we can specify variable arguments, create signatures up to this many range/value pairs
const static int MAX_RANGE_SIGNATURE_SIZE = 16;

ExpressionFunctionRange::ExpressionFunctionRange()
{
    m_functionDefinition = NULL;
}


ExpressionFunctionRange::~ExpressionFunctionRange()
{
    FDO_SAFE_RELEASE(m_functionDefinition);
}


ExpressionFunctionRange* ExpressionFunctionRange::Create()
{
    return new ExpressionFunctionRange();
}


void ExpressionFunctionRange::AddSignatures(FdoSignatureDefinitionCollection* signatures,
                   FdoArgumentDefinition* expression, FdoArgumentDefinition* defaultValue,
                   FdoArgumentDefinition* rangeMin, FdoArgumentDefinition* rangeMax,
                   FdoArgumentDefinition* value, FdoDataType returnType)
{
    for (int num=1; num<=MAX_RANGE_SIGNATURE_SIZE; num++)
    {
        FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();
        args->Add(expression);
        args->Add(defaultValue);
        for (int i=0; i<num; i++)
        {
            args->Add(rangeMin);
            args->Add(rangeMax);
            args->Add(value);
        }
        signatures->Add(FdoSignatureDefinition::Create(returnType, args));
    }
}


FdoFunctionDefinition* ExpressionFunctionRange::GetFunctionDefinition()
{
    if (!m_functionDefinition)
    {
        STRING funcDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_Description");
        STRING expVDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_ExpressionDescription");
        STRING defVDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_DefaultValueDescription");
        STRING minVDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_MinDescription");
        STRING maxVDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_MaxDescription");
        STRING valVDesc = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgFunctionRANGE_ValueDescription");

        // the expression builder treats all numerical types the same, so only use Doubles for numerical arguments
        FdoPtr<FdoArgumentDefinition> argExpString = FdoArgumentDefinition::Create(L"expression"  , expVDesc.c_str(), FdoDataType_String); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argMinString = FdoArgumentDefinition::Create(L"rangeMin"    , minVDesc.c_str(), FdoDataType_String); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argMaxString = FdoArgumentDefinition::Create(L"rangeMax"    , maxVDesc.c_str(), FdoDataType_String); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argDefString = FdoArgumentDefinition::Create(L"defaultValue", defVDesc.c_str(), FdoDataType_String); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argValString = FdoArgumentDefinition::Create(L"value"       , valVDesc.c_str(), FdoDataType_String); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argExpDouble = FdoArgumentDefinition::Create(L"expression"  , expVDesc.c_str(), FdoDataType_Double); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argDefDouble = FdoArgumentDefinition::Create(L"defaultValue", defVDesc.c_str(), FdoDataType_Double); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argValDouble = FdoArgumentDefinition::Create(L"value"       , valVDesc.c_str(), FdoDataType_Double); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argMinDouble = FdoArgumentDefinition::Create(L"rangeMin"    , minVDesc.c_str(), FdoDataType_Double); // NOXLATE
        FdoPtr<FdoArgumentDefinition> argMaxDouble = FdoArgumentDefinition::Create(L"rangeMax"    , maxVDesc.c_str(), FdoDataType_Double); // NOXLATE

        // create signatures for all combinations of keys (string, number) and values (string, number)
        // TODO: when it becomes possible, create signature with variable arguments to handle very large cases
        FdoPtr<FdoSignatureDefinitionCollection> signatures = FdoSignatureDefinitionCollection::Create();
        AddSignatures(signatures, argExpString, argDefString, argMinString, argMaxString, argValString, FdoDataType_String);
        AddSignatures(signatures, argExpString, argDefDouble, argMinString, argMaxString, argValDouble, FdoDataType_Double);
        AddSignatures(signatures, argExpDouble, argDefString, argMinDouble, argMaxDouble, argValString, FdoDataType_String);
        AddSignatures(signatures, argExpDouble, argDefDouble, argMinDouble, argMaxDouble, argValDouble, FdoDataType_Double);

        m_functionDefinition = FdoFunctionDefinition::Create(L"Range", // NOXLATE
                                                             funcDesc.c_str(),
                                                             false,
                                                             signatures,
                                                             FdoFunctionCategoryType_Conversion);
    }

    return FDO_SAFE_ADDREF(m_functionDefinition);
}


FdoLiteralValue* ExpressionFunctionRange::Evaluate(FdoLiteralValueCollection *literalValues)
{
    // make sure we have 2 or more arguments, and that there are 2 plus a multiple of 3
    if (literalValues->GetCount() < 2 || (literalValues->GetCount()-2) % 3 != 0)
    {
        MgResources* resources = MgResources::GetInstance();
        assert(NULL != resources);

        STRING message = MgUtil::GetResourceMessage(MgResources::Stylization, L"MgIncorrectNumberOfArguments");
        MgStringCollection arguments;
        arguments.Add(L"RANGE"); // NOXLATE
        message = resources->FormatMessage(message, &arguments);

        throw FdoExpressionException::Create(message.c_str());
    }

    // the first parameter is the key we seek
    FdoPtr<FdoLiteralValue> key = literalValues->GetItem(0);

    // look for our key in remaining pairs, return matching value if found
    for (int i=2; i<literalValues->GetCount(); i += 3)
    {
        FdoPtr<FdoLiteralValue> minKey = literalValues->GetItem(i);
        FdoPtr<FdoLiteralValue> maxKey = literalValues->GetItem(i+1);
        if (ExpressionHelper::Compare(key, minKey) >= 0 && ExpressionHelper::Compare(key, maxKey) < 0)
            return literalValues->GetItem(i+2);
    }

    // we didn't find a match - return the default
    return literalValues->GetItem(1);
}


FdoExpressionEngineIFunction* ExpressionFunctionRange::CreateObject()
{
    return ExpressionFunctionRange::Create();
}


void ExpressionFunctionRange::Dispose()
{
    delete this;
}
