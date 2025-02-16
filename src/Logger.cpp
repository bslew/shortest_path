#include "Logger.h"

namespace minpath {

Logger getLogger(int verbosity) {
    verbosity = spdlog::level::level_enum::n_levels -
                static_cast<spdlog::level::level_enum>(verbosity);
    auto verb = static_cast<spdlog::level::level_enum>(verbosity);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(verb);
    //    console_sink->set_level(spdlog::level::debug);
    //    console_sink->set_pattern("[test] [%^%l%$] %v");

    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("run.log", true);
    file_sink->set_level(verb);

    spdlog::logger logger("minpath", {console_sink, file_sink});
    logger.set_level(verb);

    if (verbosity > 2) {
        logger.sinks()[0]->set_level(verb);
        logger.sinks()[1]->set_level(verb);
    }

    return logger;
}

} // namespace minpath
