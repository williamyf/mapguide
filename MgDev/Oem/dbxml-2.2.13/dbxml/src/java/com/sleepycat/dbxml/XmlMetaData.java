/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.24
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.dbxml;

import com.sleepycat.db.*;
import com.sleepycat.db.internal.DbEnv;
import com.sleepycat.db.internal.DbConstants;

public class XmlMetaData {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected XmlMetaData(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(XmlMetaData obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected XmlMetaData() {
    this(0, false);
  }

  protected void finalize() {
    delete();
  }

  public void delete() /* no exception */ {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      dbxml_javaJNI.delete_XmlMetaData(swigCPtr);
    }
    swigCPtr = 0;
  }

  public String get_uri() throws XmlException {
    return dbxml_javaJNI.XmlMetaData_get_uri(swigCPtr);
  }

  public String get_name() throws XmlException {
    return dbxml_javaJNI.XmlMetaData_get_name(swigCPtr);
  }

  public XmlValue get_value() throws XmlException {
    return new XmlValue(dbxml_javaJNI.XmlMetaData_get_value(swigCPtr), false);
  }

}
