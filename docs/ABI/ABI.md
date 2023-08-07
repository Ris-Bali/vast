<!-- Autogenerated by mlir-tblgen; don't manually edit -->
# 'abi' Dialect

A vast ABI dialect.
Dialect provides operations to describe how arguments and return values
are transformed to better model target abi.

[TOC]

## Operation definition

### `abi.call_args` (::vast::abi::CallArgsOp)

Not implement yet.

WIP
#### Results:

| Result | Description |
| :----: | ----------- |
| `results` | any type

### `abi.call_exec` (::vast::abi::CallExecutionOp)

WIP

WIP
Interfaces: CallOpInterface

#### Attributes:

| Attribute | MLIR Type | Description |
| :-------: | :-------: | ----------- |
| `callee` | ::mlir::FlatSymbolRefAttr | flat symbol reference attribute

#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `args` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `result` | any type

### `abi.call` (::vast::abi::CallOp)

ABI call operation

ABI call operation
Interfaces: CallOpInterface

#### Attributes:

| Attribute | MLIR Type | Description |
| :-------: | :-------: | ----------- |
| `callee` | ::mlir::FlatSymbolRefAttr | flat symbol reference attribute

#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `args` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `results` | any type

### `abi.call_rets` (::vast::abi::CallRetsOp)

Not implement yet.

WIP
#### Results:

| Result | Description |
| :----: | ----------- |
| `results` | any type

### `abi.direct` (::vast::abi::DirectOp)

Pass value directly - usually means by register

Pass value directly - usually means by register.
#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `value` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `result` | any type

### `abi.epilogue` (::vast::abi::EpilogueOp)

WIP

WIP
#### Results:

| Result | Description |
| :----: | ----------- |
| `results` | any type

### `abi.func` (::vast::abi::FuncOp)

 Function with transformed type. 

Placeholder.

Traits: AutomaticAllocationScope, IsolatedFromAbove, NoTerminator

Interfaces: CallableOpInterface, FunctionOpInterface, Symbol

#### Attributes:

| Attribute | MLIR Type | Description |
| :-------: | :-------: | ----------- |
| `sym_name` | ::mlir::StringAttr | string attribute
| `function_type` | ::mlir::TypeAttr | type attribute of function type
| `linkage` | ::vast::hl::GlobalLinkageKindAttr | Linkage type/kind
| `sym_visibility` | ::mlir::StringAttr | string attribute
| `arg_attrs` | ::mlir::ArrayAttr | Array of dictionary attributes
| `res_attrs` | ::mlir::ArrayAttr | Array of dictionary attributes

### `abi.prologue` (::vast::abi::PrologueOp)

WIP

WIP
#### Results:

| Result | Description |
| :----: | ----------- |
| `results` | any type

### `abi.ret_direct` (::vast::abi::RetDirectOp)

Value is returned directly.

Value is returned directly.
#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `value` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `result` | any type

### `abi.todo` (::vast::abi::TodoOp)

Not implement yet.

Not implemented yet
#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `value` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `result` | any type

### `abi.wrap_fn` (::vast::abi::WrapFuncOp)

 Function that defines abi transformation of args. 

Placeholder.

Traits: AutomaticAllocationScope, IsolatedFromAbove, NoTerminator

Interfaces: CallableOpInterface, FunctionOpInterface, Symbol

#### Attributes:

| Attribute | MLIR Type | Description |
| :-------: | :-------: | ----------- |
| `sym_name` | ::mlir::StringAttr | string attribute
| `function_type` | ::mlir::TypeAttr | type attribute of function type
| `linkage` | ::vast::hl::GlobalLinkageKindAttr | Linkage type/kind
| `sym_visibility` | ::mlir::StringAttr | string attribute
| `arg_attrs` | ::mlir::ArrayAttr | Array of dictionary attributes
| `res_attrs` | ::mlir::ArrayAttr | Array of dictionary attributes

### `abi.yield` (::vast::abi::YieldOp)

WIP

WIP
Traits: Terminator

#### Operands:

| Operand | Description |
| :-----: | ----------- |
| `values` | any type

#### Results:

| Result | Description |
| :----: | ----------- |
| `result` | any type
