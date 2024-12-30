#ifndef LIBFILESYNC_PROTOCOL_MEMORY_CHAR_ARRAY_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_CHAR_ARRAY_HPP

#include <span>
#include <vector>

namespace filesync::protocol::memory {

    class CharArray {

        public:
            explicit CharArray(std::vector<char>& data);
            explicit CharArray(std::vector<char>&& data);
            std::span<char> data();

        private:
            std::vector<char> dataContainer;

    };

}

#endif