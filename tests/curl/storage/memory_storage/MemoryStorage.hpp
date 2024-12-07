#ifndef LIBFILESYNC_TESTS_CURL_STORAGE_MEMORY_STORAGE_MEMORY_STORAGE_HPP
#define LIBFILESYNC_TESTS_CURL_STORAGE_MEMORY_STORAGE_MEMORY_STORAGE_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/curl/storage/CharBuffer.hpp>

#include <string>

namespace filesync::integration_test::curl::storage::memory_storage {

    class MemoryStorage : public IntegrationTest {

        public:
            MemoryStorage(const std::string& name,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string server;
            std::string pathOnServer;

            std::string file1Name;
            std::string file1Content;
            std::string file2Name;
            std::string file2Content;

            std::string testCase2Content;

            filesync::curl::storage::CharBuffer data1;
            filesync::curl::storage::CharBuffer data2;            

            /**
             * @brief Test download to memory
             */
            void performDownload();
            void evaluateDownload();

            /**
             * @brief Test upload from memory
             */
            void performUpload();
            void evaluateUpload();

    };

}

#endif