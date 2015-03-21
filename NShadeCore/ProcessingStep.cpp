#include "stdafx.h"
#include "processingstep.h"

void ProcessingStep::AssignShaders(Shaders *texture)
{
    _shaders = shared_ptr<Shaders>(texture);
}