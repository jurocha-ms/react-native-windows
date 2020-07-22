// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <cxxreact/CxxModule.h>
#include <TestRunner.h>
#include <DevSettings.h>
#include "UniversalTestInstance.h"

using std::make_shared;
using std::make_tuple;
using std::make_unique;
using std::move;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::unique_ptr;
using std::vector;

using facebook::react::DevSettings;
using facebook::xplat::module::CxxModule;

// Stubs
namespace facebook::react {
void InitializeLogging(NativeLoggingHook &&) {}
}

namespace Microsoft::React::Test {

shared_ptr<ITestInstance> TestRunner::GetInstance(
    string &&jsBundleFile,
    vector<tuple<string, CxxModule::Provider>> &&cxxModules,
    shared_ptr<DevSettings> devSettings) noexcept {

  return shared_ptr<ITestInstance>(new UniversalTestInstance());
}

} // namespace Microsoft::React::Test
