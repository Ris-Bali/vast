// RUN: vast-cc --ccopts -xc --from-source %s | vast-opt --vast-hl-lower-types --vast-hl-to-func --vast-hl-to-ll-vars --vast-irs-to-llvm | FileCheck %s

// CHECK: llvm.func @fn() {
void fn()
{
    // CHECK: llvm.return
}
// CHECK : }