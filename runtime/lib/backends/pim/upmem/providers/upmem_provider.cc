#include "brt/core/session/session.h"
#include <memory>
#include "brt/backends/pim/upmem/providers/upmem_provider.h"
#include "brt/core/framework/kernel_registry.h"
#include "brt/backends/common.h"
#include "brt/backends/pim/upmem/device/dpu_allocator.h"
#include "brt/backends/pim/upmem/providers/gemv/op_registration.h"
#include "brt/backends/pim/upmem/providers/copy/op_registration.h"
using namespace brt;
using namespace brt::pim;
using namespace brt::common;

namespace brt {
   

   
namespace {

// statcially register all UPMEM OpKernels
// TODO: to use MACRO trick to load all kernels
// TODO: to add dynamic suppport.
// clang-format off
BRT_STATIC_KERNEL_REGISTRATION(DeviceKind::UPMEM, ProviderType::BRT, [](KernelRegistry *registry) {
      upmem::RegisterGeMVOps(registry);
      upmem::RegisterCopyOps(registry);
      RegisterCommonBuiltinOps(registry);
    });
// clang-format on

} // namespace

UPMEMExecutionProvider::UPMEMExecutionProvider(const std::string &name)
    : ExecutionProvider(DeviceKind::UPMEM, name) {}

common::Status DefaultUPMEMExecutionProviderFactory(Session *session,
                                                   int /*device_id*/) {
  // create a UPMEM provider
  auto provider = std::make_unique<UPMEMExecutionProvider>();

  // give ownership to the session
  return session->AddExecutionProvider(std::move(provider));
}

} // namespace brt