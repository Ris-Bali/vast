// RUN: %vast-cc1 -vast-emit-mlir=hl %s -o - | %vast-opt --vast-hl-lower-types --vast-hl-structs-to-llvm | %file-check %s

struct Y;

// CHECK: hl.typedef "X" : !llvm.struct<"X", (i32, ptr<struct<"Y", (f32)>>)>
struct X { int x; struct Y *y; };

// CHECK: hl.typedef "Y" : !llvm.struct<"Y", (f32)>
struct Y { float x; };
