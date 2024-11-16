#ifndef LIBFILESYNC_CORE_SYNC_DATA_FILE_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_FILE_BUFFER_HPP

#include <libfilesync/core/sync_data/Buffer.hpp>

#include <istream>

namespace filesync::core::sync_data {

    class FileBuffer : public Buffer {

        public:
            FileBuffer();

        private:
            const std::string filePath;

            void doStore(std::istream& in) override;
            bool doWriteContentTo(std::ostream& out) override;
            bool doIsEqualTo(std::istream& in) override;
            
    };

}

#endif