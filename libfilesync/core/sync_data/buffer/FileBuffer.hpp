#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP

#include <filesystem>
#include <istream>
#include <memory>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in a
     * file.
     * 
     * Patterns:
     * 
     *  - Element of visitor pattern
     */
    class FileBuffer {

        public:
            FileBuffer();
            FileBuffer(const FileBuffer& rhs);
            FileBuffer(FileBuffer&& rhs) noexcept;
            FileBuffer& operator=(FileBuffer rhs);
            ~FileBuffer();
            friend void swap(FileBuffer& lhs, FileBuffer& rhs) noexcept;

            std::unique_ptr<FileBuffer> cloneType() const;
            bool isEmpty() const;
            const std::filesystem::path&  getFilePath() const;
            void store(std::istream& in);
            std::ostream& extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;
            bool isEqualTo(FileBuffer& in) const;

        private:
            std::filesystem::path filePath;

            void resetInputStream(std::istream& in) const;
            bool hasStreamReachedEnd(std::istream& in) const;
            void printContentOfBufferAnd(std::istream& in) const;
            
    };

}

#endif