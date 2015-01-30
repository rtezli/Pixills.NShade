#pragma once
#include "targetver.h"
#include "windows.h"
#include "Unknwn.h"

#define WIN32_LEAN_AND_MEAN 

#ifndef DESTROY
#define DESTROY( obj ){ if (obj){obj->Release(); obj  = 0; }}
#endif