#pragma once

#include "targetver.h"
#include "windows.h"

#define WIN32_LEAN_AND_MEAN 
#define API
#define EXTERN

#ifndef DESTROY
#define DESTROY( obj ){ if (obj){obj->Release(); obj  = 0; }}
#endif