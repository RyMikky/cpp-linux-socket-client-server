#pragma once
#include "../includes/base_includes.h"

// Область видимости дополнительных компонентов
namespace utils {

    // Область видимости компонентов работы со временем
    namespace time {

        // Возвращает штамп с текущим временем
        static inline std::string GetCurrentTimeStamp() {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            struct tm tm;
            localtime_r(&time, &tm);

            char buffer[80];
            strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S", &tm);
            sprintf(buffer + strlen(buffer), ".%03d]", static_cast<int>(milliseconds.count()));

            return buffer;
        }

    } // namespace time

} // namespace utils