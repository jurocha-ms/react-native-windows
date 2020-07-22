// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"
#include <NativeModules.h>
#include "MockReactPackageProvider.h"

#include <TestRunner.h>

using namespace Microsoft::React::Test;

TEST_CLASS(RNTesterIntegrationTests) {

  TestRunner m_runner;

  TEST_METHOD(Dummy)
  {
    auto result = m_runner.RunTest("IntegrationTests/IntegrationTestsApp", "DummyTest");
    TestCheckEqual(static_cast<unsigned int>(TestStatus::Passed), static_cast<unsigned int>(result.Status));
  }
};
