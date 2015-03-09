#include "stdafx.h"

#include "ioc.h"

#include "memory"
#include "vector"


class IOC
{
public:
	template <typename T>
	static T* Get()
	{
		std::vector<T>* instances = IOC::GetInstances();
		std::find(instances.begin(), instances.end(), typename(o)) != instances.end();
	}

	template <typename T>
	static void Register(T o)
	{
		std::vector<void*>* instances = IOC::GetInstances();
		std::find(instances.begin(), instances.end(), typeid(o)) != instances.end();
		instances->push_back(o);
	}

	template <typename T>
	static void RegisterSingleton(T o)
	{
		std::vector<void*>* instances = IOC::GetInstances();
		std::find(instances.begin(), instances.end(), typeid(o)) != instances.end();
		instances->push_back(o);
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