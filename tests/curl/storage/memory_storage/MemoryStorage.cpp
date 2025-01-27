#include <tests/curl/storage/memory_storage/MemoryStorage.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

using namespace filesync::utility::literals;

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

        expectedRef1 = {file1Content.begin(), file1Content.end()};

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

        TestCase reupload {
            .name = "Test reupload from memory",
            .perform = std::bind(&MemoryStorage::performReUpload, this),
            .evaluate = std::bind(&MemoryStorage::evaluateReUpload, this)
        };
        addTestCase(reupload);

        TestCase largeDownload {
            .name = "Test large download to memory",
            .perform = std::bind(&MemoryStorage::performLargeDownload, this),
            .evaluate = std::bind(&MemoryStorage::evaluateLargeDownload, this)
        };
        addTestCase(largeDownload);

        TestCase largeUpload {
            .name = "Test large upload from memory",
            .perform = std::bind(&MemoryStorage::performLargeUpload, this),
            .evaluate = std::bind(&MemoryStorage::evaluateLargeUpload, this)
        };
        addTestCase(largeUpload);

    }

    void MemoryStorage::performDownload() {
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }
        {
            filesync::curl::FtpClient curlProto(server);
            curlProto.setLocalFileForUpload(file1Name);
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.upload();
        }

        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        data1 = curlProto.getDownloadAsCharVector();
    }

    void MemoryStorage::evaluateDownload() {
        bool success = true;
        if (data1.size() != expectedRef1.size()) {
            success = false;
        } else {
            for (int i = 0; i < data1.size(); i++) {
                if (data1[i] != expectedRef1[i]) {
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
            std::cout << "Expected content (size = " << expectedRef1.size() << "):" << std::endl;
            for (auto i : expectedRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Download to memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performUpload() {
        filesync::curl::FtpClient curlProto(server);

        expectedRef2 = {testCase2Content.begin(), testCase2Content.end()};
        curlProto.setInMemoryDataForUpload(expectedRef2);
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.upload();
    }

    void MemoryStorage::evaluateUpload() {
        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle1 = curlProto.takeDownloadMemory();

        std::span<const char> dataRef {dataHandle1->data()};
        bool success = true;
        if (dataRef.size() != expectedRef2.size()) {
            success = false;
        } else {
            for (int i = 0; i < dataRef.size(); i++) {
                if (dataRef[i] != expectedRef2[i]) {
                    success = false;
                }
            }
        }
        if (!success) {
            std::cout << "Uploaded content (size = " << dataRef.size() << "):" << std::endl;
            for (auto i : dataRef) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << expectedRef2.size() << "):" << std::endl;
            for (auto i : expectedRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performReUpload() {
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }
        {
            filesync::curl::FtpClient curlProto(server);
            curlProto.setLocalFileForUpload(file1Name);
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.upload();
        }

        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle1 = curlProto.takeDownloadMemory();

        curlProto.setInMemoryDataForUpload(dataHandle1->data());
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.upload();
    }

    void MemoryStorage::evaluateReUpload() {
        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle1 = curlProto.takeDownloadMemory();

        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.download();
        dataHandle2 = curlProto.takeDownloadMemory();

        std::span<char> dataRef1 {dataHandle1->data()};
        std::span<char> dataRef2 {dataHandle2->data()};

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
            std::cout << "Re-Uploaded content (size = " << dataRef1.size() << "):" << std::endl;
            for (auto i : dataRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << dataRef2.size() << "):" << std::endl;
            for (auto i : dataRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Re-Upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performReuseOfMemory() {
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }
        {
            filesync::curl::FtpClient curlProto(server);
            curlProto.setLocalFileForUpload(file1Name);
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.upload();
        }

        filesync::curl::FtpClient curlProto(server);

        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle2 = curlProto.takeDownloadMemory();

        curlProto.setInMemoryDataForUpload(dataHandle1->data());
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.upload();

        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.download();
        dataHandle2 = curlProto.takeDownloadMemory();
    }

    void MemoryStorage::evaluateReuseOfMemory() {
        
        std::span<char> dataRef1 {dataHandle2->data()};

        bool success = true;
        if (dataRef1.size() != expectedRef1.size()) {
            success = false;
        } else {
            for (int i = 0; i < dataRef1.size(); i++) {
                if (dataRef1[i] != expectedRef1[i]) {
                    success = false;
                }
            }
        }
        if (!success) {
            std::cout << "Re-Uploaded content (size = " << dataRef1.size() << "):" << std::endl;
            for (auto i : dataRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << expectedRef1.size() << "):" << std::endl;
            for (auto i : expectedRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Re-Upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performLargeDownload() {
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::filesystem::resize_file(file1Name, 1_MB);
            file1Size = std::filesystem::file_size(file1Name);
        }
        {
            filesync::curl::FtpClient curlProto(server);
            curlProto.setLocalFileForUpload(file1Name);
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.upload();
        }
        
        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle1 = curlProto.takeDownloadMemory();

    }

    void MemoryStorage::evaluateLargeDownload() {
        std::span<char> dataRef1 {dataHandle1->data()};

        std::ifstream expectedFile(file1Name);
        std::string expectedContent;

        expectedContent.assign(
            std::istreambuf_iterator<char>(expectedFile),
            std::istreambuf_iterator<char>());
        expectedRef2 = {expectedContent.begin(), expectedContent.end()};

        bool success = true;
        if (dataRef1.size() != expectedRef2.size()) {
            success = false;
        } else {
            for (int i = 0; i < dataRef1.size(); i++) {
                if (dataRef1[i] != expectedRef2[i]) {
                    success = false;
                }
            }
        }
        if (!success) {
            std::cout << "Re-Uploaded content (size = " << dataRef1.size() << "):" << std::endl;
            for (auto i : dataRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << expectedRef2.size() << "):" << std::endl;
            for (auto i : expectedRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Re-Upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

    void MemoryStorage::performLargeUpload() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::filesystem::resize_file(file1Name, 1_MB);
            file1Size = std::filesystem::file_size(file1Name);
        }
        {
            filesync::curl::FtpClient curlProto(server);

            curlProto.setLocalFileForUpload(file1Name);
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.upload();
            
            curlProto.prepareDownloadToMemory();
            curlProto.setRemoteFile(pathOnServer + separator + file1Name);
            curlProto.download();
            dataHandle1 = curlProto.takeDownloadMemory();
        }

        filesync::curl::FtpClient curlProto(server);
        curlProto.setInMemoryDataForUpload(dataHandle1->data());
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.upload();

    }

    void MemoryStorage::evaluateLargeUpload() {
        filesync::curl::FtpClient curlProto(server);
        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file1Name);
        curlProto.download();
        dataHandle1 = curlProto.takeDownloadMemory();

        curlProto.prepareDownloadToMemory();
        curlProto.setRemoteFile(pathOnServer + separator + file2Name);
        curlProto.download();
        dataHandle2 = curlProto.takeDownloadMemory();

        std::span<char> dataRef1 {dataHandle1->data()};
        std::span<char> dataRef2 {dataHandle2->data()};

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
            std::cout << "Re-Uploaded content (size = " << dataRef1.size() << "):" << std::endl;
            for (auto i : dataRef1) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << "Expected content (size = " << dataRef2.size() << "):" << std::endl;
            for (auto i : dataRef2) {
                std::cout << i;
            }
            std::cout << std::endl;
            throw FileSyncException("Large upload from memory does not have correct content.",
                __FILE__, __LINE__);
        }
    }

}