#define WIN32_LEAN_AND_MEAN 

// template<typename T>
// using POINTR = std::shared_ptr<T>;

#ifndef DESTROY
#define DESTROY( obj ){ if (obj){obj->Release(); obj  = 0; }}
#endif