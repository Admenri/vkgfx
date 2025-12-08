
#include "gfx/gfx_instance.h"

#include "webgpu/webgpu.hpp"

int main() {
  auto instance = vkgfx::GFXInstance::Create(nullptr);
  if (!instance) {
    return -1;
  }

  auto callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                     WGPUStringView message, WGPU_NULLABLE void* userdata1,
                     WGPU_NULLABLE void* userdata2) {};

  instance->RequestAdapter(
      nullptr, WGPURequestAdapterCallbackInfo{nullptr, WGPUCallbackMode(),
                                              callback, nullptr, nullptr});

  return 0;
}
