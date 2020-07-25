#pragma once

#include <NativeModules.h>
#include <ReactContext.h>

namespace Microsoft::React::Test {

REACT_MODULE(TurboTestModule, L"TestModule")
struct TurboTestModule {
  static std::function<void(bool)> OnTestCompleted;

  REACT_INIT(Initialize)
  void Initialize(::React::ReactContext const &) noexcept;

  REACT_METHOD(MarkTestPassed, L"markTestPassed")
  void MarkTestPassed(bool success) noexcept;

  REACT_METHOD(MarkTestCompleted, L"markTestCompleted")
  void MarkTestCompleted() noexcept;

  REACT_METHOD(ShouldResolve, L"shouldResolve")
  void ShouldResolve() noexcept;
};

}
