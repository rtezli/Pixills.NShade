#pragma once

#include "memory"
#include "string"
#include "dwrite.h"
#include "d2d1.h"
#include "res.h"
#include "export.h"

using namespace std;

EXTERN class API FpsTextRenderer
{
public:
    HRESULT Initialize(IDXGISwapChain *swapChain);
private:
    wstring                             _text;
    DWRITE_TEXT_METRICS	                _textMetrics;
    shared_ptr<ID2D1SolidColorBrush>    _whiteBrush;
    shared_ptr<ID2D1DrawingStateBlock>  _stateBlock;
    shared_ptr<IDWriteTextLayout>       _textLayout;
    shared_ptr<IDWriteTextFormat>       _textFormat;
};