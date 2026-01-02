// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_utils.h"

namespace vkgfx {

VkCompareOp ToVulkanCompareOp(WGPUCompareFunction op) {
  switch (op) {
    case WGPUCompareFunction_Never:
      return VK_COMPARE_OP_NEVER;
    case WGPUCompareFunction_Less:
      return VK_COMPARE_OP_LESS;
    case WGPUCompareFunction_LessEqual:
      return VK_COMPARE_OP_LESS_OR_EQUAL;
    case WGPUCompareFunction_Greater:
      return VK_COMPARE_OP_GREATER;
    case WGPUCompareFunction_GreaterEqual:
      return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case WGPUCompareFunction_Equal:
      return VK_COMPARE_OP_EQUAL;
    case WGPUCompareFunction_NotEqual:
      return VK_COMPARE_OP_NOT_EQUAL;
    case WGPUCompareFunction_Always:
      return VK_COMPARE_OP_ALWAYS;

    case WGPUCompareFunction_Undefined:
      break;
  }
  return VkCompareOp();
}

}  // namespace vkgfx
