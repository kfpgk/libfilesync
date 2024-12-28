#ifndef LIBFILESYNC_TESTS_CURL_STORAGE_MEMORY_STORAGE_MEMORY_STORAGE_HPP
#define LIBFILESYNC_TESTS_CURL_STORAGE_MEMORY_STORAGE_MEMORY_STORAGE_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>

#include <memory>
#include <span>
#include <string>
#include <vector>

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

            std::span<char> dataRef1;
            std::span<char> dataRef2;

            std::vector<char> data1;
            std::vector<char> data2;

            std::unique_ptr<filesync::curl::storage::MemoryStorageHandle> dataHandle1;
            std::unique_ptr<filesync::curl::storage::MemoryStorageHandle> dataHandle2;            

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

            /**
             * @brief Test reupload from memory
             */
            void performReUpload();
            void evaluateReUpload();

    };

}

#endif