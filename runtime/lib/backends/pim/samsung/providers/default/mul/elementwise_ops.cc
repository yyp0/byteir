
#include "./elementwise_ops.h"
#include "./mul.h"
#include "FP16.h"
#include "brt/backends/pim/samsung/device/BurstTensor.h"
#include "brt/backends/pim/samsung/device/hbm_worker_queue.h"
#include "brt/core/context/execution_context.h"
#include "brt/core/context/execution_frame.h"
#include "brt/core/context/work_queue.h"
#include "brt/core/framework/op_accessor.h"
#include "brt/core/ir/ir.h"
#include "brt/core/ir/util.h"

#include <utility>

using namespace brt;
using namespace brt::common;

using namespace brt::ir;
using namespace llvm;
using namespace mlir;
using namespace brt::pim::hbmpim;
namespace brt {
namespace pim {
namespace hbmpim {

template <typename T> common::Status Mul<T>::ProloguePerSession() {
  std::cout << "this is CustomizeMulOp ProloguePerSession" << std::endl;
  return Status::OK();
}

template <typename T>
common::Status Mul<T>::ProloguePerFrame(const ExecutionContext &ctx) {
  brt::ExecutionFrame::StateInfo &state_info = ctx.frame_state_info;

  std::string space = OpAccessor(info_).GetAttrAsString("device");

  IAllocator *alloc = info_.GetAllocator(space);
  if (!alloc)
    return common::Status(common::StatusCategory::BRT, common::StatusCode::FAIL,
                          "Cannot find allocator");

  // status = state_info.CreateStateIfNotExist(
  //     GetAITOpKernelRunnerUniqueKey(), ctx.exec_frame, [=]() {
  //       return static_cast<void *>(new AITOpKernelRunner(
  //           aitLibHdl, workspaceMgr, alloc, space, workspaceSizeInBytes,
  //           name));
  //     });
}
template <typename T>
Mul<T>::Mul(const OpKernelInfo &info)
    : OpKernel(info, false, false, true, true) {
  OpAccessor accessor(info_);
  std::string ir_path = info_.GetIRPath();
  // std::string lib_path = brt::ir::GetParentPath(ir_path);
  //  lib_path += accessor.GetAttrAsString(std::string("hbm_lib_file"));
  std::string space = accessor.GetAttrAsString("device");
  // IAllocator *alloc = info_.GetAllocator(space);
}
template <typename T>
common::Status Mul<T>::RunImpl(const ExecutionContext &ctx) {
  OpAccessor accessor(info_, ctx.exec_frame);
  auto work_queue = static_cast<HBMPIMWorkQueue *>(ctx.work_queue);
  // TODO move the following to util

  T *A = static_cast<T *>(accessor.GetArgAsyncValueRef(0));
  T *B = static_cast<T *>(accessor.GetArgAsyncValueRef(1));

  // T *C = static_cast<T *>(accessor.Get(0));
  T *C = static_cast<T *>(accessor.GetArgAsyncValueRef(0));

  auto a_shape = accessor.GetArgShape(0);
  auto b_shape = accessor.GetArgShape(1);
  // auto c_shape = accessor.GetArgShape(2);
  int m = a_shape[0];
  int n = b_shape[1];

  uint32_t output_dim = m * n;
  vector<float> c = vector<float>(A, A + output_dim);
  vector<float> d = vector<float>(B, B + output_dim);
  auto kernel = *static_cast<HBMPIMWorkQueue *>(ctx.work_queue)->Getkernel();

  // BurstTensor<T> C(output_dim);

  TDataDim *dim_data =
      new TDataDim(KernelType::MUL, 1, output_dim, output_dim, true, c, d, c);

  kernel.preloadNoReplacement(&dim_data->input_npbst_, 0, 0);
  kernel.preloadNoReplacement(&dim_data->input1_npbst_, 0, 0);

  DRAMSim::BurstType *r = new DRAMSim::BurstType[output_dim];

  kernel::mul_kernel<T>(kernel, dim_data, r);
  work_queue->AddTask(0, NULL, NULL);

  return common::Status::OK();
};
template class Mul<float>;
template class Mul<int>;
template class Mul<half_float::half>;
} // namespace hbmpim
} // namespace pim
} // namespace brt