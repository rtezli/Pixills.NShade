// dllmain.h : Declaration of module class.

class CNShadeATLModule : public ATL::CAtlDllModuleT< CNShadeATLModule >
{
public :
	DECLARE_LIBID(LIBID_NShadeATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NSHADEATL, "{2244DFC8-BF95-45F6-9EC8-C581AD1B3D28}")
};

extern class CNShadeATLModule _AtlModule;
