#include "stdafx.h"
#include "cppunittest.h"
#include "..\NShadeCore\D3DSystem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NShadeTests
{
    TEST_CLASS(D3DSystemTests)
    {
    public:
        TEST_METHOD(InitializeTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(D3DConstructorTestTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(InitializeForWindowTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(InitializeWithWindowTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(MessageHandlerTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }
    };
}