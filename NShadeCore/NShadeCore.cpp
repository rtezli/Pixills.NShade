#include "stdafx.h"
#include "NShadeCore.h"


// This is an example of an exported variable
int nNShadeCore=0;

// This is an example of an exported function.
int fnNShadeCore(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see NShadeCore.h for the class definition
CNShadeCore::CNShadeCore()
{
	return;
}
