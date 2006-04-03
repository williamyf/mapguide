//
// See the file LICENSE for redistribution information.
//
// Copyright (c) 2002-2005
//	Sleepycat Software.  All rights reserved.
//
// $Id: XmlData.hpp,v 1.3 2005/04/05 16:43:53 bostic Exp $
//

#ifndef __XMLDATA_HPP
#define	__XMLDATA_HPP

#include <db_cxx.h>

#include "XmlPortability.hpp"

namespace DbXml
{

class DBXML_EXPORT XmlData
{
public:
  XmlData();
  XmlData(const Dbt &dbt);
  XmlData(void *data, u_int32_t size);
  XmlData(const XmlData &o);
  XmlData &operator=(const XmlData &o);
  virtual ~XmlData();

  void *get_data() const;
  void set_data(void *value);

  u_int32_t get_size() const;
  void set_size(u_int32_t size);

  /** @name Berkeley DB Compatibility */

  Dbt &getDbt();
  const Dbt &getDbt() const;

private:
  Dbt *dbt_;
};

}

#endif
