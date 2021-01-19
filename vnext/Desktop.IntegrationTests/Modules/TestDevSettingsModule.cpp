// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "TestDevSettingsModule.h"

using namespace facebook::xplat;
using namespace folly;

using std::map;
using std::string;
using std::vector;

namespace Microsoft::React::Test {

/*static*/ string TestDevSettingsModule::name = "DevSettings";

#pragma region CxxModule methods

string TestDevSettingsModule::getName() {
  return name;
}

map<string, dynamic> TestDevSettingsModule::getConstants() {
  return {};
}

vector<module::CxxModule::Method> TestDevSettingsModule::getMethods() {
  return {
      {"addMenuItem",
       [](dynamic /*args*/) {

       }},
      {"reload",
       [](dynamic /*args*/) {

       }},
      {"onFastRefresh",
       [](dynamic /*args*/) {

       }},
  };
}

#pragma endregion
} // namespace Microsoft::React::Test
