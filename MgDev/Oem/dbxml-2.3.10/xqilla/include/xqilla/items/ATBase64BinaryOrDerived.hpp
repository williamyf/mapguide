/*
 * Copyright (c) 2001-2006
 *     DecisionSoft Limited. All rights reserved.
 * Copyright (c) 2004-2006
 *     Progress Software Corporation. All rights reserved.
 * Copyright (c) 2004-2006
 *     Oracle. All rights reserved.
 *
 * See the file LICENSE for redistribution information.
 *
 * $Id: ATBase64BinaryOrDerived.hpp,v 1.8 2006/11/01 16:37:14 jpcs Exp $
 */

#ifndef _ATBASE64BINARYORDERIVED_HPP
#define _ATBASE64BINARYORDERIVED_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xqilla/items/AnyAtomicType.hpp>

#include <xqilla/framework/XQillaExport.hpp>

class DynamicContext;

class XQILLA_API ATBase64BinaryOrDerived : public AnyAtomicType
{
public:
  /* Get the name of the primitive type (basic type) of this type (ie "decimal" for xs:decimal) */
  virtual const XMLCh* getPrimitiveTypeName() const = 0;

  /* Get the namespace URI for this type */
  virtual const XMLCh* getTypeURI() const = 0;

  /* Get the name of this type  (ie "integer" for xs:integer) */
  virtual const XMLCh* getTypeName() const = 0;

  /* returns the XMLCh* (canonical) representation of this type */
  virtual const XMLCh* asString(const DynamicContext* context) const = 0;

  /* returns true if the two objects' base 64 binary representation
   *  are equal (string comparison) false otherwise */
  virtual bool equals(const AnyAtomicType::Ptr &target, const DynamicContext* context) const = 0;

  /** Returns less than 0 if this is less that other,
      0 if they are the same, and greater than 0 otherwise */
  virtual int compare(const ATBase64BinaryOrDerived::Ptr &other, const DynamicContext *context) const = 0;
 
protected: 
  virtual AnyAtomicType::AtomicObjectType getPrimitiveTypeIndex() const = 0;
};

#endif //  _ATBASE64BINARYORDERIVED_HPP
