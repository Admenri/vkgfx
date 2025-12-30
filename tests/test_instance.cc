
#include <iostream>

#include "webgpu/webgpu-raii.hpp"

int main() {
  wgpu::raii::Instance instance = wgpu::createInstance();

  wgpu::RequestAdapterOptions adapter_options;
  wgpu::raii::Adapter adapter = instance->requestAdapter(adapter_options);

  wgpu::DeviceDescriptor device_desc;
  wgpu::raii::Device device = adapter->requestDevice(device_desc);

  wgpu::AdapterInfo adapter_info;
  adapter->getInfo(&adapter_info);

  std::cout << "[Device] " << adapter_info.device.data << '\n';

  return 0;
}
