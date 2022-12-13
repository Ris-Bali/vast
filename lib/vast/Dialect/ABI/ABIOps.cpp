// Copyright (c) 2022-present, Trail of Bits, Inc.

#include "vast/Util/Warnings.hpp"

#include "vast/Dialect/ABI/ABIDialect.hpp"
#include "vast/Dialect/ABI/ABIOps.hpp"

VAST_RELAX_WARNINGS
#include <mlir/IR/Builders.h>
VAST_UNRELAX_WARNINGS

#define GET_OP_CLASSES
#include "vast/Dialect/ABI/ABI.cpp.inc"