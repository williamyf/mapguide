/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.38
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.db.internal;

import com.sleepycat.db.*;
import java.util.Comparator;

public class DbLock {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected DbLock(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(DbLock obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    try {
      delete();
    } catch(Exception e) {
      System.err.println("Exception during finalization: " + e);
      e.printStackTrace(System.err);
    }
  }

  /* package */ synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      db_javaJNI.delete_DbLock(swigCPtr);
    }
    swigCPtr = 0;
  }

	public Lock wrapper;

}
