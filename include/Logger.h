#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#pragma once
namespace minpath {
using Logger = spdlog::logger;

Logger getLogger(int verbosity = 3);

} // namespace minpath