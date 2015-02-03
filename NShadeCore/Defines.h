#define WIN32_LEAN_AND_MEAN 

#ifndef DESTROY
#define DESTROY( obj ){ if (obj){obj->Release(); obj  = 0; }}
#endif