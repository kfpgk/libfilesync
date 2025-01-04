#ifndef LIBFILESYNC_CURL_UTILITY_DEBUG_HPP
#define LIBFILESYNC_CURL_UTILITY_DEBUG_HPP

#include <libfilesync/curl/config/config.h>

#ifdef LIBFILESYNC_CURL_UTILITY_DEBUG_BUILD
#   define LIBFILESYNC_CURL_UTILITY_DEBUG(x) do { \
                std::cerr << x << std::endl; \
            } while (0)
#   define LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER() do { \
                std::cerr << "Enter " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; \
            } while (0)
#   define LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT() do { \
                std::cerr << "Exit " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; \
            } while (0)
#else
#   define LIBFILESYNC_CURL_UTILITY_DEBUG(x) do {} while (0)
#   define LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER() do {} while (0)
#   define LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT() do {} while (0)
#endif

#endif