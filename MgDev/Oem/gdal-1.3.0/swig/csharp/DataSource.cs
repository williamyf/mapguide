/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.25
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


using System;
using System.Runtime.InteropServices;

public class DataSource : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal DataSource(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(DataSource obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      throw new MethodAccessException("C++ destructor does not have public access");
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public string name {
    get {
      string ret = ogrPINVOKE.get_DataSource_name(swigCPtr);
      return ret;
    } 
  }

  public void Destroy() {
    ogrPINVOKE.DataSource_Destroy(swigCPtr);
  }

  public void Release() {
    ogrPINVOKE.DataSource_Release(swigCPtr);
  }

  public int Reference() {
    int ret = ogrPINVOKE.DataSource_Reference(swigCPtr);
    return ret;
  }

  public int Dereference() {
    int ret = ogrPINVOKE.DataSource_Dereference(swigCPtr);
    return ret;
  }

  public int GetRefCount() {
    int ret = ogrPINVOKE.DataSource_GetRefCount(swigCPtr);
    return ret;
  }

  public int GetSummaryRefCount() {
    int ret = ogrPINVOKE.DataSource_GetSummaryRefCount(swigCPtr);
    return ret;
  }

  public int GetLayerCount() {
    int ret = ogrPINVOKE.DataSource_GetLayerCount(swigCPtr);
    return ret;
  }

  public string GetName() {
    string ret = ogrPINVOKE.DataSource_GetName(swigCPtr);
    return ret;
  }

  public int DeleteLayer(int index) {
    int ret = ogrPINVOKE.DataSource_DeleteLayer(swigCPtr, index);
    return ret;
  }

  public Layer CreateLayer(string name, SWIGTYPE_p_OSRSpatialReferenceShadow reference, int geom_type, SWIGTYPE_p_p_char options) {
    IntPtr cPtr = ogrPINVOKE.DataSource_CreateLayer(swigCPtr, name, SWIGTYPE_p_OSRSpatialReferenceShadow.getCPtr(reference), geom_type, SWIGTYPE_p_p_char.getCPtr(options));
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false);
    return ret;
  }

  public Layer CopyLayer(Layer src_layer, string new_name, SWIGTYPE_p_p_char options) {
    IntPtr cPtr = ogrPINVOKE.DataSource_CopyLayer(swigCPtr, Layer.getCPtr(src_layer), new_name, SWIGTYPE_p_p_char.getCPtr(options));
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false);
    return ret;
  }

  public Layer GetLayerByIndex(int index) {
    IntPtr cPtr = ogrPINVOKE.DataSource_GetLayerByIndex(swigCPtr, index);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false);
    return ret;
  }

  public Layer GetLayerByName(string layer_name) {
    IntPtr cPtr = ogrPINVOKE.DataSource_GetLayerByName(swigCPtr, layer_name);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false);
    return ret;
  }

  public int TestCapability(string cap) {
    int ret = ogrPINVOKE.DataSource_TestCapability(swigCPtr, cap);
    return ret;
  }

  public Layer ExecuteSQL(string statement, Geometry geom, string dialect) {
    IntPtr cPtr = ogrPINVOKE.DataSource_ExecuteSQL(swigCPtr, statement, Geometry.getCPtr(geom), dialect);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, true);
    return ret;
  }

  public void ReleaseResultSet(Layer layer) {
    ogrPINVOKE.DataSource_ReleaseResultSet(swigCPtr, Layer.getCPtr(layer));
  }

}
