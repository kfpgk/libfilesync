#ifndef LIBFILESYNC_TESTS_CORE_SYNC_DATA_FILE_BUFFER_HPP
#define LIBFILESYNC_TESTS_CORE_SYNC_DATA_FILE_BUFFER_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integrationtest::core::sync_data::file_buffer {

    class FileBuffer : public IntegrationTest {

        public:
            explicit FileBuffer(const std::string& testName);

        private:
            const std::string file1Name;
            const std::string file1Content;
            const std::string file2Name;
            const std::string file2Content;

            void setup() override;
            void perform() override;
            void evaluate() override;

    };

}

#endif