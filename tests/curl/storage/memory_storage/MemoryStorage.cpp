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

        data2.write("file1 content");

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
        data1.clear();  
        curlProto.prepareDownloadToMemory(data1);
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
    }

    void MemoryStorage::evaluateDownload() {
        if (data1 != data2) {
            data1.print();
            throw FileSyncException("Download to memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performUpload() {
        filesync::curl::FtpClient curlProto(server);
        data1.clear();
        data1.write(testCase2Content);
        data1.print();  
        curlProto.setInMemoryDataForUpload(data1);
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.upload();
    }

    void MemoryStorage::evaluateUpload() {
        filesync::curl::FtpClient curlProto(server);
        data2.clear();  
        curlProto.prepareDownloadToMemory(data2);
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();

        if (data1 != data2) {
            data2.print();
            throw FileSyncException("Upload from memory did not result in expected content.",
                __FILE__, __LINE__);
        }
    }

}