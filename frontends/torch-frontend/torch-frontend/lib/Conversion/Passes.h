//===- Passes.h ----------------------------------------------*--- C++ -*-===//
//
// Copyright 2022 ByteDance Ltd. and/or its affiliates. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//

#ifndef TORCH_FRONTEND_CONVERSION_PASSES
#define TORCH_FRONTEND_CONVERSION_PASSES

#include "torch-frontend/Conversion/ConvertTorchToCustomCall.h"
#include "torch-frontend/Conversion/ConvertTorchToHBMPIMCustomCall.h"
#include "torch-frontend/Conversion/ConvertTorchToStablehloExt.h"
#include "torch-frontend/Conversion/FuseOpOnTorch.h"

namespace mlir {

class Module;

namespace func {
class FuncOp;
} // namespace func

// Generate the code for registering conversion passes.
#define GEN_PASS_REGISTRATION
#include "torch-frontend/Conversion/Passes.h.inc"

} // namespace mlir

#endif // TORCH_FRONTEND_CONVERSION_PASSES