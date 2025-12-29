
#include <iostream>

#include "webgpu/webgpu.hpp"

int main() {
  wgpu::Instance instance = wgpu::createInstance();

  wgpu::RequestAdapterOptions adapter_options;
  auto adapter = instance.requestAdapter(adapter_options);

  wgpu::DeviceDescriptor device_desc;
  auto device = adapter.requestDevice(device_desc);

  wgpu::AdapterInfo adapter_info;
  adapter.getInfo(&adapter_info);

  std::cout << "[Device] " << adapter_info.device.data << '\n';

  return 0;
}
