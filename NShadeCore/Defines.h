#pragma once
#pragma warning( disable : 4615 )

#define WIN32_LEAN_AND_MEAN

#define DIRECTINPUT_VERSION 0x0800

#ifndef FPS
#define FPS(fps) {std::chrono::milliseconds(1000 / fps)}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif