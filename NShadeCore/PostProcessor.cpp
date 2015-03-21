#include "stdafx.h"
#include "postprocessor.h"

void PostProcessor::AddProcessingStep(ProcessingStep *step)
{
    if (!_processingSteps)
    {
        _processingSteps = new vector<ProcessingStep*>();
    }
    _processingSteps->push_back(step);
}

void Apply()
{
    
}