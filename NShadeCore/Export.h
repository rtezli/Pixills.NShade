#pragma once
#pragma warning( disable: 4251 )
#pragma warning( disable: 4005 )

#ifdef API
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#ifdef EXTERN
#define EXTERN extern "C"
#else
#define EXTERN extern "C"
#endif