#ifndef LIBFILESYNC_UTILITY_DEBUG_HPP
#define LIBFILESYNC_UTILITY_DEBUG_HPP

#include <libfilesync/config/config.h>

#include <iostream>

#ifdef DEBUG_BUILD
#   define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#   define DEBUG_ENTER() do { std::cerr << "Enter " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; } while (0)
#   define DEBUG_EXIT() do { std::cerr << "Exit " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; } while (0)
#else
#   define DEBUG(x) do {} while (0)
#   define DEBUG_ENTER() do {} while (0)
#   define DEBUG_EXIT() do {} while (0)
#endif

#endif