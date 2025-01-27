#include <tests/protocol/ftp/performance/UploadDownloadPerformance.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>

using namespace filesync::utility;
using namespace filesync::utility::literals;

namespace filesync::integration_test::protocol::ftp::performance {

    UploadDownloadPerformance::UploadDownloadPerformance(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            ServerAccess(testName, server, pathOnServer) {

        TestCase uploadFromFile {
            .name = "Test performance of upload from file",
            .perform = std::bind(&UploadDownloadPerformance::performUploadFromFile, this),
            .evaluate = std::bind(&UploadDownloadPerformance::evaluateUploadFromFile, this)
        };
        addTestCase(uploadFromFile);

        TestCase downloadToFile {
            .name = "Test performance of download to file",
            .perform = std::bind(&UploadDownloadPerformance::performDownloadToFile, this),
            .evaluate = std::bind(&UploadDownloadPerformance::evaluateDownloadToFile, this)
        };
        addTestCase(downloadToFile);

        TestCase downloadToMemory {
            .name = "Test performance of download to memory",
            .perform = std::bind(&UploadDownloadPerformance::performDownloadToMemory, this),
            .evaluate = std::bind(&UploadDownloadPerformance::evaluateDownloadToMemory, this)
        };
       addTestCase(downloadToMemory);

        TestCase uploadFromMemory {
            .name = "Test performance of upload from memory",
            .perform = std::bind(&UploadDownloadPerformance::performUploadFromMemory, this),
            .evaluate = std::bind(&UploadDownloadPerformance::evaluateUploadFromMemory, this)
        };
        addTestCase(uploadFromMemory); 

   }

    void UploadDownloadPerformance::setup() {

        std::ofstream largeFile(uploadFileName);

        std::filesystem::resize_file(uploadFileName, 10_MB);

    }

    void UploadDownloadPerformance::performUploadFromFile() {

        filesync::protocol::FtpClient ftpClient(server, pathOnServer);

        using namespace std::chrono;
        auto start = steady_clock::now();
      
        ftpClient.upload(uploadFileName, remoteFileName);       

        auto end = steady_clock::now();

        executionTime = duration_cast<milliseconds>(end - start);

    }

    void UploadDownloadPerformance::evaluateUploadFromFile() {

        Logger::getInstance().log(LogDomain::TestResult,
            "Upload from " 
            + std::to_string(std::filesystem::file_size(uploadFileName) / 1024 / 1024)
            + "MB file took: " + std::to_string(executionTime.count()) + "ms");

        using namespace std::chrono;
        assert(executionTime < 5s);

    }

    void UploadDownloadPerformance::performDownloadToFile() {

        filesync::protocol::FtpClient ftpClient(server, pathOnServer);

        if (!ftpClient.existsOnServer(remoteFileName)) {
            ftpClient.upload(uploadFileName, remoteFileName);
        }   

        using namespace std::chrono;
        auto start = steady_clock::now();

        ftpClient.download(downloadFileName, remoteFileName);       

        auto end = steady_clock::now();

        executionTime = duration_cast<milliseconds>(end - start);

    }

    void UploadDownloadPerformance::evaluateDownloadToFile() {

        if (!filesync::data::areEqual(
            uploadFileName, downloadFileName)) {

            throw FileSyncException("Downloaded file does not match uploaded file",
                __FILE__, __LINE__);   
        }

        Logger::getInstance().log(LogDomain::TestResult,
            "Download of " 
            + std::to_string(std::filesystem::file_size(uploadFileName) / 1024 / 1024)
            + "MB file took: " + std::to_string(executionTime.count()) + "ms");

        using namespace std::chrono;
        assert(executionTime < 5s);

    }

    void UploadDownloadPerformance::performDownloadToMemory() {

        filesync::protocol::FtpClient ftpClient(server, pathOnServer);

        if (!ftpClient.existsOnServer(remoteFileName)) {
            ftpClient.upload(uploadFileName, remoteFileName);
        }   

        using namespace std::chrono;
        auto start = steady_clock::now();

        ftpClient.download(handle, remoteFileName);        

        auto end = steady_clock::now();

        executionTime = duration_cast<milliseconds>(end - start);        

    }

    void UploadDownloadPerformance::evaluateDownloadToMemory() {

        if (std::filesystem::file_size(uploadFileName) !=
            handle->data().size()) {

            throw FileSyncException("Size of downloaded memory not as expected",
                __FILE__, __LINE__);            
        }

        Logger::getInstance().log(LogDomain::TestResult,
            "Download of " 
            + std::to_string(std::filesystem::file_size(uploadFileName) / 1024 / 1024)
            + "MB file to memory took: " + std::to_string(executionTime.count()) + "ms");

        using namespace std::chrono;
        assert(executionTime < 5s);

    }

    void UploadDownloadPerformance::performUploadFromMemory() {

        filesync::protocol::FtpClient ftpClient(server, pathOnServer);

        if (!handle) {
            ftpClient.upload(uploadFileName, remoteFileName);
            ftpClient.download(handle, remoteFileName);
        }   

        using namespace std::chrono;
        auto start = steady_clock::now();

        ftpClient.upload(handle, remoteFileName);        

        auto end = steady_clock::now();

        executionTime = duration_cast<milliseconds>(end - start); 

    }

    void UploadDownloadPerformance::evaluateUploadFromMemory() {

        Logger::getInstance().log(LogDomain::TestResult,
            "Upload of "
            + std::to_string(std::filesystem::file_size(uploadFileName) / 1024 / 1024)
            + "MB file from memory took: " + std::to_string(executionTime.count()) + "ms");

        using namespace std::chrono;
        assert(executionTime < 5s);

    }

    void UploadDownloadPerformance::cleanUp() {
        filesync::protocol::FtpClient client(server, pathOnServer);

        if (client.existsOnServer(remoteFileName)) {
            client.deleteOnServer(remoteFileName);
        }       
      
    }

}