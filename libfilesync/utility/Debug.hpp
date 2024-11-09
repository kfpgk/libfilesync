#ifndef LIBFILESYNC_UTILITY_DEBUG_HPP
#define LIBFILESYNC_UTILITY_DEBUG_HPP

#include <libfilesync/config/config.h>

#include <iostream>
#include <string>

namespace filesync::utility {

    extern thread_local size_t debugIndent;

}


#ifdef DEBUG_BUILD
#   define DEBUG(x) do { \
                std::cerr << std::string(filesync::utility::debugIndent, ' ' ); \
                std::cerr << x << std::endl; \
            } while (0)
#   define DEBUG_ENTER() do { \
                std::cerr << std::string(filesync::utility::debugIndent, ' ' ); \
                std::cerr << "Enter " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; \
                filesync::utility::debugIndent++; \
            } while (0)
#   define DEBUG_EXIT() do { \
                filesync::utility::debugIndent--; \
                std::cerr << std::string(filesync::utility::debugIndent, ' '); \
                std::cerr << "Exit " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; \
            } while (0)
#else
#   define DEBUG(x) do {} while (0)
#   define DEBUG_ENTER() do {} while (0)
#   define DEBUG_EXIT() do {} while (0)
#endif

#endif