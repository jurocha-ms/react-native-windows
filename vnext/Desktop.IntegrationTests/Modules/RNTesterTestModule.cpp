// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "RNTesterTestModule.h"

#include <cxxreact/JsArgumentHelpers.h>

using namespace facebook::xplat;
using namespace folly;

using std::map;
using std::string;
using std::vector;

namespace Microsoft::React::Test {

/*static*/ string RNTesterTestModule::name = "RNTesterTestModule";

#pragma region CxxModule methods

string RNTesterTestModule::getName() {
  return name;
}

map<string, dynamic> RNTesterTestModule::getConstants() {
  return {};
}

vector<module::CxxModule::Method> RNTesterTestModule::getMethods() {
  return {
      {"echoString",
       [](dynamic args) -> dynamic {
      return dynamic(jsArgAsString(args, 0));
       },
       SyncTag
    },
      {"methodThatReturnsNil",
       []() -> dynamic { return dynamic(nullptr);
       },
         SyncTag
       },

      //TODO: how to pass a function??? See RCTResponseSenderBlock
      {"methodThatCallsCallbackWithString",
       [](dynamic /*args*/) -> dynamic {
          return {};
       }, SyncTag},
  };
}

#pragma endregion
} // namespace Microsoft::React::Test
