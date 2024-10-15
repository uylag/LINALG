#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "include/linalg/linalg.hxx"
#include "include/linalg/matrix.hxx"

int main() {
    auto file_logger = spdlog::basic_logger_mt(
        "global_logger", \
        "/home/uylag/LINALG/logs/math.log"
    );

    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);

    if (!file_logger) {
        std::cerr << "Failed to create logger!\n";
        return 1;
    }

    file_logger->info("Logger initialized successfully!");

    linalg::Matrix<long double> matrix({
        {1, 0, 3},
        {4, 5, 7},
        {0, 0, 1},
        {0, 0, 4}
    }, "CRS");

    matrix.set(0, 1, 5);
    file_logger->debug(matrix.print());

    file_logger->flush();
    spdlog::shutdown();

    return 0;
}