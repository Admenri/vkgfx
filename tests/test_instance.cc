
#include <iostream>

#include "webgpu/webgpu_cpp.hpp"

int main() {
  auto instance = wgpu::CreateInstance(nullptr);

  wgpu::Adapter adapter = nullptr;
  instance.RequestAdapter(
      nullptr,
      {
          .callback =
              [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                 WGPUStringView message, void* userdata1, void* userdata2) {
                *reinterpret_cast<wgpu::Adapter*>(userdata1) =
                    wgpu::Adapter::Acquire(adapter);
              },
          .userdata1 = &adapter,
      });

  wgpu::Device device = nullptr;
  adapter.RequestDevice(
      nullptr,
      {
          .callback =
              [](WGPURequestDeviceStatus status, WGPUDevice device,
                 WGPUStringView message, void* userdata1, void* userdata2) {
                *reinterpret_cast<wgpu::Device*>(userdata1) =
                    wgpu::Device::Acquire(device);
              },
          .userdata1 = &device,
      });

  wgpu::AdapterInfo adapter_info = {};
  adapter.GetInfo(&adapter_info);
  std::cout << "[Device] " << std::string(adapter_info.device) << '\n';

  return 0;
}
