#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP

#include <filesystem>
#include <istream>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in a
     * file.
     * 
     * Patterns:
     *  - Element of visitor pattern
     */
    class FileBuffer {

        public:
            FileBuffer();

            const std::filesystem::path&  getFilePath() const;
            void store(std::istream& in);
            std::ostream& extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;

        private:
            const std::filesystem::path filePath;
            
    };

}

#endif