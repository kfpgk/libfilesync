#ifndef LIBFILESYNC_UTILITY_LITERALS_HPP
#define LIBFILESYNC_UTILITY_LITERALS_HPP

namespace filesync::utility {

    namespace literals {

        constexpr std::size_t operator""_KB(unsigned long long int x) {
            return 1024ULL * x;
        }

        constexpr std::size_t operator""_MB(unsigned long long int x) {
            return 1024_KB * x;
        }

        constexpr std::size_t operator""_GB(unsigned long long int x) {
            return 1024_MB * x;
        }

        constexpr std::size_t operator""_TB(unsigned long long int x) {
            return 1024_GB * x;
        }

    }

}

#endif