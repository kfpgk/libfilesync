#include <tests/curl/storage/memory_storage/MemoryStorage.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/core/sync_data/File.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <fstream>
#include <memory>

namespace filesync::integration_test::curl::storage::memory_storage {

    constexpr auto separator = std::filesystem::path::preferred_separator;

    MemoryStorage::MemoryStorage(const std::string& name,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest{name},
            server{server},
            pathOnServer{pathOnServer},
            file1Name{"file1"},
            file1Content{"file1 content"},
            file2Name{"file2"},
            file2Content{"file2 content"},
            testCase2Content{"test case 2 file content"} {

        dataRef2 = {file1Content.begin(), file1Content.end()};

        TestCase download {
            .name = "Test download to memory",
            .perform = std::bind(&MemoryStorage::performDownload, this),
            .evaluate = std::bind(&MemoryStorage::evaluateDownload, this)
        };
        addTestCase(download);

        TestCase upload {
            .name = "Test upload from memory",
            .perform = std::bind(&MemoryStorage::performUpload, this),
            .evaluate = std::bind(&MemoryStorage::evaluateUpload, this)
        };
        addTestCase(upload);

    }

    void MemoryStorage::performDownload() {
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.upload(file1Name);

        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        data1 = curlProto.getCopyOfDownloadedMemory();
    }

    void MemoryStorage::evaluateDownload() {
        bool success = true;
        if (data1.size() != dataRef2.size()) {
            success = false;
        } else {
            for (int i = 0; i < data1.size(); i++) {
                if (data1[i] != dataRef2[i]) {
                    success = false;
                }
            }
        }
        if (!success) {
            std::cout << "Downloaded content (size = " << data1.size() << "):" << std::endl;
            for (auto i : data1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << dataRef2.size() << "):" << std::endl;
            for (auto i : dataRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Download to memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performUpload() {
        filesync::curl::FtpClient curlProto(server);

        dataRef1 = {testCase2Content.begin(), testCase2Content.end()};
        curlProto.setInMemoryDataForUpload(dataRef1);
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.upload();
    }

    void MemoryStorage::evaluateUpload() {
        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataRef2 = curlProto.getReferenceToDownloadedMemory();

        bool success = true;
        if (dataRef1.size() != dataRef2.size()) {
            success = false;
        } else {
            for (int i = 0; i < dataRef1.size(); i++) {
                if (dataRef1[i] != dataRef2[i]) {
                    success = false;
                }
            }
        }
        if (!success) {
            std::cout << "Uploaded content (size = " << data1.size() << "):" << std::endl;
            for (auto i : dataRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << dataRef2.size() << "):" << std::endl;
            for (auto i : dataRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

}