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

#include "gfx/common/log.h"

#include "gfx/common/platform.h"

namespace vkgfx {

namespace {

const char* SeverityName(LogSeverity severity) {
  switch (severity) {
    case LogSeverity::kDebug:
      return "Debug";
    case LogSeverity::kInfo:
      return "Info";
    case LogSeverity::kWarning:
      return "Warning";
    case LogSeverity::kError:
      return "Error";
    default:
      return "Unknown";
  }
}

#if GFX_PLATFORM_IS(ANDROID) && !defined(GFX_DISABLE_LOGGING)
android_LogPriority AndroidLogPriority(LogSeverity severity) {
  switch (severity) {
    case LogSeverity::kDebug:
      return ANDROID_LOG_INFO;
    case LogSeverity::kInfo:
      return ANDROID_LOG_INFO;
    case LogSeverity::kWarning:
      return ANDROID_LOG_WARN;
    case LogSeverity::kError:
      return ANDROID_LOG_ERROR;
    default:
      return ANDROID_LOG_ERROR;
  }
}
#endif  // GFX_PLATFORM_IS(ANDROID) && !defined(GFX_DISABLE_LOGGING)

}  // anonymous namespace

#if defined(GFX_DISABLE_LOGGING)
LogMessage::~LogMessage() {
  (void)mSeverity;
  return;
}
#else  // defined(GFX_DISABLE_LOGGING)
LogMessage::~LogMessage() {
  std::string fullMessage = mStream.str();

  // If this message has been moved, its stream is empty.
  if (fullMessage.empty()) {
    return;
  }

  const char* severityName = SeverityName(mSeverity);

#if GFX_PLATFORM_IS(ANDROID)
  android_LogPriority androidPriority = AndroidLogPriority(mSeverity);
  __android_log_print(androidPriority, "Dawn", "%s: %s\n", severityName,
                      fullMessage.c_str());
#else   // GFX_PLATFORM_IS(ANDROID)
  FILE* outputStream = stdout;
  if (mSeverity == LogSeverity::kWarning || mSeverity == LogSeverity::kError)
    outputStream = stderr;

  // Note: we use fprintf because <iostream> includes static initializers.
  fprintf(outputStream, "%s: %s\n", severityName, fullMessage.c_str());
  fflush(outputStream);
#endif  // GFX_PLATFORM_IS(ANDROID)
}
#endif  // defined(GFX_DISABLE_LOGGING)

}  // namespace vkgfx
