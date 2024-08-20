//===- Translation.h ------------------------------------------*--- C++ -*-===//
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

#ifndef BYTEIR_C_TRANSLATION_H
#define BYTEIR_C_TRANSLATION_H

#include "mlir-c/IR.h"
#include "mlir-c/Support.h"

enum ByteIRHead {
  TOTAL_HEAD_BYTES = 12,
  MAGIC_NUMBER = 0x30dc4790,
  MAGIC_NUMBER_BYTES = 4,
  MAJOR_VERSION = 1,
  MAJOR_VERSION_BYTES = 4,
  MINOR_VERSION = 0,
  MINOR_VERSION_BYTES = 4,
};

#ifdef __cplusplus
extern "C" {
#endif

MLIR_CAPI_EXPORTED void byteirRegisterTranslationDialects(MlirContext context);

MLIR_CAPI_EXPORTED bool byteirTranslateToPTX(MlirModule module,
                                             MlirStringRef ptxFilePrefixName,
                                             MlirStringRef gpuArch);

MLIR_CAPI_EXPORTED bool byteirTranslateToLLVMBC(MlirModule module,
                                                MlirStringRef outputFile);

MLIR_CAPI_EXPORTED bool byteirTranslateToLLVMIR(MlirModule module,
                                                MlirStringRef outputFile);

MLIR_CAPI_EXPORTED bool byteirSerializeByre(MlirModule module,
                                            MlirStringRef targetVersion,
                                            MlirStringRef outputFile);

MLIR_CAPI_EXPORTED MlirModule byteirDeserializeByre(MlirStringRef artifactStr,
                                                    MlirContext context);

MLIR_CAPI_EXPORTED MlirModule byteirMergeTwoModules(MlirModule module0,
                                                    MlirModule module1);

#ifdef __cplusplus
}
#endif

#endif // BYTEIR_C_TRANSLATION_H
