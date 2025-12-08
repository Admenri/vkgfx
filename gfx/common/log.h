// Copyright 2017 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef GFX_COMMON_LOG_H_
#define GFX_COMMON_LOG_H_

#include <sstream>

namespace vkgfx {

enum class LogSeverity {
  kInfo = 0,
  kDebug,
  kWarning,
  kError,
  kNums,
};

class LogMessage {
 public:
  explicit LogMessage(LogSeverity severity) : mSeverity(severity) {}
  LogMessage(LogMessage&& other) = default;
  LogMessage& operator=(LogMessage&& other) = default;

  ~LogMessage();

  template <typename T>
  LogMessage& operator<<(T&& value) {
    mStream << value;
    return *this;
  }

 private:
  LogMessage(const LogMessage& other) = delete;
  LogMessage& operator=(const LogMessage& other) = delete;

  LogSeverity mSeverity;
  std::ostringstream mStream;
};

#define GFX_INFO() ::vkgfx::LogMessage(LogSeverity::kInfo)
#define GFX_DEBUG() ::vkgfx::LogMessage(LogSeverity::kDebug)
#define GFX_WARNING() ::vkgfx::LogMessage(LogSeverity::kWarning)
#define GFX_ERROR() ::vkgfx::LogMessage(LogSeverity::kError)

}  // namespace vkgfx

#endif  //! GFX_COMMON_LOG_H_