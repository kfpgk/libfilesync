#include <libfilesync/utility/Logger.hpp>

#include <sstream>
#include <iostream>

namespace filesync::utility {

    Logger::Logger() {
        
    }

    Logger& Logger::getInstance() {
        static Logger logger;
        return logger;
    }

    void Logger::log(LogDomain domain, const std::string& text) const {
        std::cout << text << std::endl;
    }

    void Logger::log(LogDomain domain,
        const std::string& text,
        const char* file,
        int line) const {

        std::cout << std::string(file) << ": ";
        std::cout << std::to_string(line) << ":\n" << text << std::endl;
    }

    void Logger::logFileWriteAccess(
        LogDomain domain,
        const std::string& name,
        const std::filesystem::file_time_type& file_time) const {
            std::cout << "'" << name << "' changed at: '";
            std::cout << timeFormat::timeToString(file_time) << "'" << std::endl;
    }

    void Logger::logOnce(LogDomain domain, const std::string& text) {
        std::string log = text;
        if (!loggedOnceMessages.contains(log)) {
            std::cout << log << std::endl;
            loggedOnceMessages.insert(log);
        }      
    }

}