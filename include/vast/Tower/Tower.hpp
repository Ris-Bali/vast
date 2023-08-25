// Copyright (c) 2022-present, Trail of Bits, Inc.

#pragma once

#include "vast/Util/Common.hpp"

VAST_RELAX_WARNINGS
#include <mlir/Pass/PassManager.h>
VAST_UNRELAX_WARNINGS

namespace vast::tower {
    struct handle_t {
        std::size_t id;
        vast_module mod;
    };

    struct default_loc_rewriter_t {
        static auto insert(mlir::Operation *op) -> void;
        static auto remove(mlir::Operation *op) -> void;
        static auto prev(mlir::Operation *op) -> mlir::Operation *;
    };

    using pass_ptr_t = std::unique_ptr< mlir::Pass >;

    template< typename loc_rewriter_t >
    struct manager_t {
        using loc_rewriter = loc_rewriter_t;

        static auto get(mcontext_t &ctx, owning_module_ref mod)
            -> std::tuple< manager_t, handle_t > {
            manager_t m(ctx, std::move(mod));
            handle_t h{ .id = 0, .mod = m._modules[0].get() };
            return { std::move(m), h };
        }

        auto apply(handle_t handle, mlir::PassManager &pm) -> handle_t {
            handle.mod.walk(loc_rewriter::insert);

            _modules.emplace_back(mlir::cast< vast_module >(handle.mod->clone()));

            auto id  = _modules.size() - 1;
            auto mod = _modules.back().get();

            VAST_CHECK(mlir::succeeded(pm.run(mod)), "Some pass in apply() failed");

            handle.mod.walk(loc_rewriter::remove);

            return { id, mod };
        }

        auto apply(handle_t handle, pass_ptr_t pass) -> handle_t {
            mlir::PassManager pm(&_ctx);
            pm.addPass(std::move(pass));
            return apply(handle, pm);
        }

      private:
        using module_storage_t = llvm::SmallVector< owning_module_ref, 2 >;

        mcontext_t &_ctx;
        module_storage_t _modules;

        manager_t(mcontext_t &ctx, owning_module_ref mod) : _ctx(ctx) {
            _modules.emplace_back(std::move(mod));
        }
    };

    using default_manager_t = manager_t< default_loc_rewriter_t >;
} // namespace vast::tower