#include "stdafx.h"
#include "cppunittest.h"
#include "..\NShadeCore\D3DSystem.h"

#pragma comment(lib, "Microsoft.VisualStudio.TestTools.CppUnitTestFramework.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NShadeTests
{
    TEST_CLASS(D3DSystemTests)
    {
    public:
        TEST_METHOD(D3DSystem_InitializeTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(D3DSystem_D3DConstructorTestTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(D3DSystem_InitializeForWindowTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(D3DSystem_InitializeWithWindowTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }

        TEST_METHOD(D3DSystem_MessageHandlerTest)
        {
            auto d3d = new D3DSystem();
            Assert::IsNotNull(d3d);
        }
    };
}