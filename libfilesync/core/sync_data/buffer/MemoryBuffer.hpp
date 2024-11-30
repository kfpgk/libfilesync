#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_MEMORY_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_MEMORY_BUFFER_HPP

#include <filesystem>
#include <istream>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in memory.
     * 
     * Patterns:
     *  - Element of visitor pattern
     */
    class MemoryBuffer {

        public:
            MemoryBuffer();

            void* getData() const;
            void store(std::istream& in);
            bool extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;

        private:
            void* data;
            size_t size;
            
    };

}

#endif