
#include <iostream>

#include "webgpu-headers/webgpu.h"

int main() {
  auto instance = wgpuCreateInstance(nullptr);

  WGPUAdapter adapter = nullptr;
  WGPURequestAdapterOptions adapter_options = {};
  wgpuInstanceRequestAdapter(
      instance, &adapter_options,
      {
          .callback =
              [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                 WGPUStringView message, WGPU_NULLABLE void* userdata1,
                 WGPU_NULLABLE void* userdata2) {
                *reinterpret_cast<WGPUAdapter*>(userdata1) = adapter;
              },
          .userdata1 = &adapter,
      });

  WGPUDevice device = nullptr;
  WGPUDeviceDescriptor device_descriptor = {};
  wgpuAdapterRequestDevice(
      adapter, &device_descriptor,
      {
          .callback =
              [](WGPURequestDeviceStatus status, WGPUDevice device,
                 WGPUStringView message, WGPU_NULLABLE void* userdata1,
                 WGPU_NULLABLE void* userdata2) {
                *reinterpret_cast<WGPUDevice*>(userdata1) = device;
              },
          .userdata1 = &device,
      });

  WGPUAdapterInfo adapter_info = {};
  wgpuAdapterGetInfo(adapter, &adapter_info);
  std::cout << "[Device] " << adapter_info.device.data << '\n';
  wgpuAdapterInfoFreeMembers(adapter_info);

  wgpuDeviceRelease(device);
  wgpuAdapterRelease(adapter);
  wgpuInstanceRelease(instance);

  return 0;
}
