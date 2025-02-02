// Copyright (c) 2021-present, Trail of Bits, Inc.

#include "vast/Dialect/HighLevel/Passes.hpp"

VAST_RELAX_WARNINGS
#include <mlir/Analysis/DataLayoutAnalysis.h>
#include <mlir/IR/PatternMatch.h>
#include <mlir/Transforms/GreedyPatternRewriteDriver.h>
#include <mlir/Transforms/DialectConversion.h>

#include <mlir/Conversion/LLVMCommon/Pattern.h>
VAST_UNRELAX_WARNINGS

#include "PassesDetails.hpp"

#include "vast/Dialect/HighLevel/HighLevelOps.hpp"
#include "vast/Dialect/LowLevel/LowLevelOps.hpp"

#include "vast/Util/Common.hpp"
#include "vast/Util/DialectConversion.hpp"
#include "vast/Util/LLVMTypeConverter.hpp"
#include "vast/Util/Symbols.hpp"

namespace vast
{
    namespace pattern
    {
        // Inline the region that is responsible for initialization
        //  * `rewriter` insert point is invalidated (although documentation of called
        //    methods does not state it, experimentally it is corrupted)
        //  * terminator is returned to be used & erased by caller.
        template< typename T >
        T inline_init_region(auto src, auto &rewriter)
        {
            auto &init_region = src.getInitializer();
            auto &init_block = init_region.back();

            auto terminator = mlir::dyn_cast< T >(init_block.getTerminator());

            rewriter.inlineRegionBefore(init_region, src->getBlock());
            rewriter.inlineBlockBefore(&init_block, src.getOperation());
            return terminator;
        }

        template< typename O >
        struct BasePattern : mlir::ConvertOpToLLVMPattern< O >
        {
            using Base = mlir::ConvertOpToLLVMPattern< O >;

            tc::LLVMTypeConverter &tc;

            BasePattern(tc::LLVMTypeConverter &tc_) : Base(tc_), tc(tc_) {}
            tc::LLVMTypeConverter &type_converter() const { return tc; }
        };

        struct vardecl_op : BasePattern< hl::VarDeclOp >
        {
            using op_t = hl::VarDeclOp;
            using Base = BasePattern< op_t >;
            using Base::Base;

            mlir::LogicalResult matchAndRewrite(
                op_t op, typename op_t::Adaptor ops,
                conversion_rewriter &rewriter) const override
            {
                auto trg_type = op.getType();

                auto uninit_var = rewriter.create< ll::UninitializedVar >(op.getLoc(),
                                                                          trg_type);

                if (op.getInitializer().empty())
                {
                    rewriter.replaceOp(op, uninit_var);
                    return mlir::success();
                }

                // This deals with cases where the initializer references
                // the variable itself - int *x = malloc(sizeof(*x));
                // We can't reference the initialized value so we use the
                // initialized one
                auto fix_init_refs = [&](){
                    auto var = op.getResult();
                    for (auto user : op->getUsers()) {
                        if (op->isAncestor(user)) {
                            for (op_operand& operand : user->getOpOperands()) {
                                if (operand.is(var)) {
                                    user->setOperand(operand.getOperandNumber(), uninit_var);
                                }
                            }
                        }
                    }
                };
                rewriter.updateRootInPlace(op, fix_init_refs);

                auto yield = inline_init_region< hl::ValueYieldOp >(op, rewriter);
                rewriter.setInsertionPointAfter(yield);
                auto initialize = rewriter.create< ll::InitializeVar >(
                        yield.getLoc(),
                        trg_type,
                        uninit_var, yield.getResult());

                rewriter.replaceOp(op, initialize);
                rewriter.eraseOp(yield);

                return mlir::success();
            }
        };

    } // namespace pattern

    struct HLToLLVarsPass : HLToLLVarsBase< HLToLLVarsPass >
    {
        void runOnOperation() override
        {
            auto op = this->getOperation();
            auto &mctx = this->getContext();

            mlir::ConversionTarget trg(mctx);
            trg.markUnknownOpDynamicallyLegal( [](auto) { return true; } );
            trg.addDynamicallyLegalOp< hl::VarDeclOp >([&](hl::VarDeclOp op)
            {
                // TODO(conv): `!ast_node->isLocalVarDeclOrParam()` should maybe be ported
                //             to the mlir op?
                return mlir::isa< vast_module >(op->getParentOp());
            });

            const auto &dl_analysis = this->getAnalysis< mlir::DataLayoutAnalysis >();

            mlir::LowerToLLVMOptions llvm_options(&mctx);
            llvm_options.useBarePtrCallConv = true;
            tc::LLVMTypeConverter type_converter(&mctx, llvm_options, &dl_analysis);

            mlir::RewritePatternSet patterns(&mctx);

            patterns.add< pattern::vardecl_op >(type_converter);

            if (mlir::failed(mlir::applyPartialConversion(op, trg, std::move(patterns))))
                return signalPassFailure();
        }
    };
} // namespace vast


std::unique_ptr< mlir::Pass > vast::createHLToLLVarsPass()
{
    return std::make_unique< vast::HLToLLVarsPass >();
}
