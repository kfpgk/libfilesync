#include <libfilesync/utility/TimeFormat.hpp>

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

namespace filesync::utility {

    /**
     * @brief Convert a file_time_type to a string
     * 
     * A better implementation requires <format>.
     * Not used due to inavailability.
     */
    std::string timeFormat::timeToString(
        const std::filesystem::file_time_type& timeToPrint) {

        time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::file_clock::to_sys(timeToPrint));
        std::string str = asctime(std::localtime(&cftime));
        str.pop_back(); // remove the trailing '\n' put by `asctime`
        return str;
    }

}