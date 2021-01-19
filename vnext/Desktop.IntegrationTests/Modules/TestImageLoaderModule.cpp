// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "TestImageLoaderModule.h"

using namespace facebook::xplat;
using namespace folly;

using std::map;
using std::string;
using std::vector;

namespace Microsoft::React::Test {

/*static*/ string TestImageLoaderModule::name = "ImageLoader";

#pragma region CxxModule methods

string TestImageLoaderModule::getName() {
  return name;
}

map<string, dynamic> TestImageLoaderModule::getConstants() {
  return {};
}

vector<module::CxxModule::Method> TestImageLoaderModule::getMethods() {
  return {
      {"getSize",
       [](dynamic /*args*/, Callback successCallback, Callback errorCallback) {

       }},
      {"getSizeWithHeaders",
       [](dynamic /*args*/, Callback successCallback, Callback errorCallback) {

       }},
      {"prefetchImage",
       [](dynamic /*args*/, Callback successCallback, Callback errorCallback) {

       }},
      {"queryCache",
       [](dynamic /*args*/, Callback successCallback, Callback errorCallback) {

       }},
  };
}

#pragma endregion
} // namespace Microsoft::React::Test
