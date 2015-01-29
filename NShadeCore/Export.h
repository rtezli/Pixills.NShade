#pragma once
#pragma warning( disable: 4251 )

#ifdef NSHADECORE_EXPORTS
#define NSHADECORE_API __declspec(dllexport)
#else
#define NSHADECORE_API __declspec(dllimport)
#endif