#define WIN32_LEAN_AND_MEAN 

// template<typename T>
// using POINTR = std::shared_ptr<T>;

#ifndef DESTROY
#define DESTROY( obj ){ if (obj){obj->Release(); obj  = 0; }}
#endif

//static void tracef(LPCTSTR lpszFormat, ...)
//{
//#ifdef _DEBUG
//	static const int BUFFERSIZE = 0x800;
//	TCHAR    lpszBuffer[BUFFERSIZE];
//	va_list  fmtList;
//
//	va_start(fmtList, lpszFormat);
//	_vstprintf_s(lpszBuffer, lpszFormat, fmtList);
//	va_end(fmtList);
//
//	OutputDebugString(lpszBuffer);
//#endif
//}