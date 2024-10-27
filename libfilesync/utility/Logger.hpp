#ifndef LIBFILESYNC_UTILITY_LOGGER_HPP
#define LIBFILESYNC_UTILITY_LOGGER_HPP

#include <libfilesync/utility/TimeFormat.hpp>

#include <string>
#include <sstream>
#include <set>
#include <filesystem>

namespace filesync::utility {

    enum class LogDomain {Debug, Error, Warning, Info, TestResult};

    /**
     * Logger utility class
     * 
     * Patterns:
     *  - Singleton
     */
    class Logger {

        public:
            static Logger& getInstance();
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            void log(LogDomain domain, const std::string& text) const;
            void log(LogDomain domain,
                const std::string& text,
                const char* file,
                int line) const;
            void logFileWriteAccess(LogDomain Domain,
                const std::string& name,
                const std::filesystem::file_time_type& file_time) const;
            void logOnce(LogDomain domain, const std::string& text);

        private:
            std::set<std::string> loggedOnceMessages;

            Logger();      

    };

}

#endif