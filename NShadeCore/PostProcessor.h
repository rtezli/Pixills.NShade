#pragma once

#include "vector"
#include "memory"
#include "res.h"
#include "processingstep.h"

using namespace std;

class PostProcessor
{
public:
    void                        AddProcessingStep(ProcessingStep *step);
    void                        Apply();
    vector<ProcessingStep*>*    const GetProcessingSteps(){ return _processingSteps; }
private:
    vector<ProcessingStep*>* _processingSteps;
};