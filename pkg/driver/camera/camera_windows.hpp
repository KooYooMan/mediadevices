#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int width;
  int height;
  size_t buf;  // uintptr

  char* name;
  void* grabber;
  void* mediaControl;
  void* callback;
} camera;

int openCamera(camera* cam, const char** errstr);
void freeCamera(camera* cam);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <windows.h>
#include <string>
#include <dshow.h>
std::string utf16Decode(LPOLESTR olestr);
IPin* getPin(IBaseFilter* filter, PIN_DIRECTION dir);
HRESULT cbBuffer(double sampleTime, BYTE* buf, long len);

template <class T>
void safeRelease(T** p)
{
  if (*p)
  {
    (*p)->Release();
    *p = nullptr;
  }
}

class SampleGrabberCallback : public ISampleGrabberCB
{
private:
  camera* cam_;

public:
  inline SampleGrabberCallback(camera* cam)
    : cam_(cam)
  {
  }

  HRESULT SampleCB(double sampleTime, IMediaSample* sample) final;
  HRESULT BufferCB(double sampleTime, BYTE* buffer, LONG bufferLen) final;

  inline ULONG STDMETHODCALLTYPE AddRef() final
  {
    return 2;
  }
  inline ULONG STDMETHODCALLTYPE Release() final
  {
    return 1;
  }
  inline HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) final
  {
    if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown)
    {
      *ppv = (void*)static_cast<ISampleGrabberCB*>(this);
      return NOERROR;
    }
    return E_NOINTERFACE;
  }
};

EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;

const static char* errAddFilter = "failed to add filter";
const static char* errConnectFilters = "failed to connect filters";
const static char* errEnumDevice = "failed to enum device";
const static char* errGrabber = "failed to create grabber";
const static char* errGraphBuilder = "failed to build graph";
const static char* errNoControl = "failed to control media";
const static char* errTerminator = "failed to create graph terminator";

#endif
