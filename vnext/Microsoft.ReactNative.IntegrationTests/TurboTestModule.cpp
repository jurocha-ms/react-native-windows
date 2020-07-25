#include "pch.h"

#include "TurboTestModule.h"

namespace Microsoft::React::Test {

/*static*/ std::function<void(bool)> TurboTestModule::OnTestCompleted;

//REACT_INIT(Initialize)
void TurboTestModule::Initialize(::React::ReactContext const &) noexcept {}

//REACT_METHOD(MarkTestPassed, L"markTestPassed")
void TurboTestModule::MarkTestPassed(bool success) noexcept {
  TurboTestModule::OnTestCompleted(success);
}

//REACT_METHOD(MarkTestCompleted, L"markTestCompleted")
void TurboTestModule::MarkTestCompleted() noexcept {
  MarkTestPassed(true);
}

//REACT_METHOD(ShouldResolve, L"shouldResolve")
void TurboTestModule::ShouldResolve() noexcept {}

}
