#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_CHAR_ARRAY_MEMORY_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_CHAR_ARRAY_MEMORY_BUFFER_HPP

#include <istream>
#include <span>
#include <vector>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in memory
     * in form of a character container.
     * 
     * Patterns:
     * 
     *  - Element of visitor pattern
     */
    class CharArrayMemoryBuffer {

        public:
            CharArrayMemoryBuffer() = default;

            std::span<char> getData();
            void store(std::istream& in);
            std::ostream& extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;

        private:
            std::vector<char> data;

            std::streampos getInStreamSize(std::istream& in) const;
            
    };

}

#endif