#pragma once

#include "memory"
#include "vector"

//#include "cpplinq.hpp"
//using namespace cpplinq;

class IOC
{
public:
	template <typename T>
	static T* Get()
	{
		std::vector<T>* instances = IOC::GetInstances();
		//return (T*)(from(instances) >> first_or_default([&](void* obj) {return typeid(obj) == typeid(T); }));
		return nullptr;
	}

	template <typename T>
	static void Register(T* o)
	{
		std::vector<void*>* instances = IOC::GetInstances();
		//T* any = (T*)(from(instances) >> first_or_default([&](void* obj) {return typeid(obj) == typeid(T); }));
		//if (!any)
		//{
		//	instances->push_back(o);
		//}
	}

	template <typename T>
	static void RegisterSingleton(T* o)
	{
		std::vector<void*>* instances = IOC::GetInstances();
		//T* any = (T*)(from(instances) >> first_or_default([&](void* obj) {return typeid(obj) == typeid(T); }));
		//if (!any)
		//{
		//	instances->push_back(o);
		//}
	}
	~IOC();
private:
	IOC();
private:
	static std::vector<void*>* _instances;
	static std::vector<void*>* GetInstances()
	{
		if (!_instances)
		{
			_instances = new std::vector<void*>();
		}
		return _instances;
	};
};