#ifndef LIBFILESYNC_UTILITY_TIME_FORMAT_HPP
#define LIBFILESYNC_UTILITY_TIME_FORMAT_HPP

#include <filesystem>

namespace filesync::utility {

    namespace timeFormat {

            [[nodiscard]] 
            std::string timeToString(
                const std::filesystem::file_time_type& timeToPrint);

    }

}

#endif