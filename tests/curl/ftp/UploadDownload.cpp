#include <tests/curl/ftp/UploadDownload.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>

namespace filesync::integration_test::curl::ftp {

    constexpr auto separator = std::filesystem::path::preferred_separator;

    UploadDownload::UploadDownload(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer},
            file1Name{"file1"},
            file1RemotePath{pathOnServer + separator + file1Name},
            file1Content{"file1 content"},
            file2Name{"file2"},
            file2RemotePath{pathOnServer + separator + file2Name},
            file2Content{"file2 content"},
            binaryFile1Name{"file1.bin"},
            binaryFile1RemotePath{pathOnServer + separator + binaryFile1Name},
            binaryFile1Content{42},
            binaryFile2Name{"file2.bin"} {

        TestCase uploadUninitialized {
            .name = "Upload without defining local and remote paths",
            .perform = std::bind(&UploadDownload::performUploadUninitialized, this),
            .evaluate = std::bind(&UploadDownload::evaluateUploadUninitialized, this)
        };
        addTestCase(uploadUninitialized);

        TestCase uploadInitNonExistingLocal {
            .name = "Upload with local file not existing during initialization",
            .perform = std::bind(&UploadDownload::performUploadInitNonExistingLocal, this),
            .evaluate = std::bind(&UploadDownload::evaluateUploadInitNonExistingLocal, this)
        };
        addTestCase(uploadInitNonExistingLocal);

        TestCase uploadNonExistingLocal {
            .name = "Upload non existing local file",
            .perform = std::bind(&UploadDownload::performUploadNonExistingLocal, this),
            .evaluate = std::bind(&UploadDownload::evaluateUploadNonExistingLocal, this)
        };
        addTestCase(uploadNonExistingLocal);

        TestCase uploadInitializedForDownload {
            .name = "Upload local file initialized for download",
            .perform = std::bind(&UploadDownload::performUploadInitializedForDownload, this),
            .evaluate = std::bind(&UploadDownload::evaluateUploadInitializedForDownload, this)
        };
        addTestCase(uploadInitializedForDownload);

        TestCase downloadUninitialized {
            .name = "Download without defining local and remote paths",
            .perform = std::bind(&UploadDownload::performDownloadUninitialized, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownloadUninitialized, this)
        };
        addTestCase(downloadUninitialized);

        TestCase downloadNonExistingRemote {
            .name = "Download non existing remote file",
            .perform = std::bind(&UploadDownload::performDownloadNonExistingRemote, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownloadNonExistingRemote, this)
        };
        addTestCase(downloadNonExistingRemote);

        TestCase downloadUndefinedRemote {
            .name = "Download with undefined remote path",
            .perform = std::bind(&UploadDownload::performDownloadUndefinedRemote, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownloadUndefinedRemote, this)
        };
        addTestCase(downloadUndefinedRemote);

        TestCase downloadInitializedForUpload {
            .name = "Download local file initialized for upload",
            .perform = std::bind(&UploadDownload::performDownloadInitializedForUpload, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownloadInitializedForUpload, this)
        };
        addTestCase(downloadInitializedForUpload);

        TestCase upload {
            .name = "upload to server",
            .perform = std::bind(&UploadDownload::performUpload, this),
            .evaluate = std::bind(&UploadDownload::evaluateUpload, this)
        };
        addTestCase(upload);

        TestCase download {
            .name = "download from server",
            .perform = std::bind(&UploadDownload::performDownload, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownload, this)
        };
        addTestCase(download);

        TestCase uploadBinaryFile {
            .name = "upload binary file to server",
            .perform = std::bind(&UploadDownload::performUploadBinaryFile, this),
            .evaluate = std::bind(&UploadDownload::evaluateUploadBinaryFile, this)
        };
        addTestCase(uploadBinaryFile);

        TestCase downloadBinaryFile {
            .name = "download binary file from server",
            .perform = std::bind(&UploadDownload::performDownloadBinaryFile, this),
            .evaluate = std::bind(&UploadDownload::evaluateDownloadBinaryFile, this)
        };
        addTestCase(downloadBinaryFile);

    }

    void UploadDownload::setup() {
        std::ofstream localFile(file1Name);
        if (!localFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        localFile << file1Content << std::endl;
        localFile.close();

        std::ofstream binFile(binaryFile1Name, std::ios::binary | std::ios::out);
        binFile.write(reinterpret_cast<char*>(&binaryFile1Content), sizeof(int));
    }

    void UploadDownload::performUploadUninitialized() {
        filesync::curl::FtpClient client(server);

        exceptionThrown = false;
        try {
            client.upload();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
    }

    void UploadDownload::evaluateUploadUninitialized() {
        if (!exceptionThrown) {
            throw FileSyncException("Uninitialized upload() unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }          
    }

    void UploadDownload::performUploadInitNonExistingLocal() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file2RemotePath);
       
        exceptionThrown = false;
        try {
            client.setLocalFileForUpload(file2Name);
            client.upload();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
        
    }

    void UploadDownload::evaluateUploadInitNonExistingLocal() {
        if (!exceptionThrown) {
            throw FileSyncException("upload() with non existing local file during "\
                "initialization unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }          
    }

    void UploadDownload::performUploadNonExistingLocal() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file2RemotePath);
        std::ofstream localFile(file2Name);
        localFile.close();
        client.setLocalFileForUpload(file2Name);
        std::remove(file2Name.c_str());
       
        client.upload();
        
    }

    void UploadDownload::evaluateUploadNonExistingLocal() {
        filesync::curl::FtpClient client(server);
        client.setRemoteFile(file2RemotePath);
        if (!client.remoteEntryExists()) {
            throw FileSyncException("upload() with non existing local file during "\
                "upload did not upload empty file as expected.",
                __FILE__, __LINE__);
        }
        client.deleteRemoteFile();          
    }

    void UploadDownload::performUploadInitializedForDownload() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file2RemotePath);
        client.createLocalFileForDownload(file2Name);

        try {
            client.upload();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
    }

    void UploadDownload::evaluateUploadInitializedForDownload() {
        if (!exceptionThrown) {
            throw FileSyncException("upload() with local file path initialized "\
            "for download unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }        
    }

    void UploadDownload::performDownloadUninitialized() {
        filesync::curl::FtpClient client(server);

        exceptionThrown = false;
        try {
            client.download();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
    }

    void UploadDownload::evaluateDownloadUninitialized() {
        if (!exceptionThrown) {
            throw FileSyncException("download() with uninitialized local and remote paths"\
            "unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }            
    }

    void UploadDownload::performDownloadNonExistingRemote() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file2RemotePath);
        client.createLocalFileForDownload(file2Name);
        
        exceptionThrown = false;
        try {
            client.download();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
        
    }

    void UploadDownload::evaluateDownloadNonExistingRemote() {
        if (!exceptionThrown) {
            throw FileSyncException("download() non existing remote file unexpectedly "\
                "did not throw exception.",
                __FILE__, __LINE__);
        }          
    }

    void UploadDownload::performDownloadUndefinedRemote() {
        filesync::curl::FtpClient client(server);

        client.createLocalFileForDownload(file2Name);

        exceptionThrown = false;
        try {
            client.download();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
        
    }

    void UploadDownload::evaluateDownloadUndefinedRemote() {
        std::remove("localfile"); 
        if (!exceptionThrown) {
            throw FileSyncException("download() with undefined remote path"\
                "unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }     
    }

    void UploadDownload::performDownloadInitializedForUpload() {
        filesync::curl::FtpClient client(server);

        std::ofstream localFile(file2Name);
        client.setLocalFileForUpload(file2Name);
        localFile.close();
        std::remove(file2Name.c_str());

        exceptionThrown = false;
        try {
            client.download();
        } catch (const std::exception& e) {
            exceptionThrown = true;
        }
    }

    void UploadDownload::evaluateDownloadInitializedForUpload() {
        if (!exceptionThrown) {
            throw FileSyncException("download() with local path initialized for "\
                "upload unexpectedly did not throw exception.",
                __FILE__, __LINE__);
        }         
    }

    void UploadDownload::performUpload() {
        filesync::curl::FtpClient client(server);

        client.setLocalFileForUpload(file1Name);
        client.setRemoteFile(file1RemotePath);
        client.upload();
    }

    void UploadDownload::evaluateUpload() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file1RemotePath);
        if (!client.remoteEntryExists()) {
            throw FileSyncException("File not found on server after upload",
                __FILE__, __LINE__);
        }
        client.deleteRemoteFile();
    }

    void UploadDownload::performDownload() {
        
        auto localSetup = [this]() {
            filesync::curl::FtpClient client(server);
            client.setLocalFileForUpload(file1Name);
            client.setRemoteFile(file1RemotePath);
            client.upload();
        };

        localSetup();

        filesync::curl::FtpClient client(server);
        client.createLocalFileForDownload(file2Name);
        client.setRemoteFile(file1RemotePath);
        client.download();

    }

    void UploadDownload::evaluateDownload() {
        std::ifstream localFile(file2Name);
        if (!localFile.is_open()) {
            throw FileSyncException("Could not open file for reading",
                __FILE__, __LINE__);
        }
        std::string line;
        std::getline(localFile, line);
        if (line != file1Content) {
            throw FileSyncException("Content of downloaded file did not meet expectation");
        }
    }

    void UploadDownload::performUploadBinaryFile() {

        filesync::curl::FtpClient client(server);

        client.setLocalFileForUpload(binaryFile1Name);
        client.setRemoteFile(binaryFile1RemotePath);
        client.upload();

    }

    void UploadDownload::evaluateUploadBinaryFile() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(binaryFile1RemotePath);
        if (!client.remoteEntryExists()) {
            throw FileSyncException("File not found on server after upload",
                __FILE__, __LINE__);
        }
        client.deleteRemoteFile();
    }

    void UploadDownload::performDownloadBinaryFile() {

        auto localSetup = [this]() {
            filesync::curl::FtpClient client(server);
            client.setLocalFileForUpload(binaryFile1Name);
            client.setRemoteFile(binaryFile1RemotePath);
            client.upload();
        };

        localSetup();

        filesync::curl::FtpClient client(server);
        client.createLocalFileForDownload(binaryFile2Name);
        client.setRemoteFile(binaryFile1RemotePath);
        client.download();
    }

    void UploadDownload::evaluateDownloadBinaryFile() {
        std::ifstream binFile(binaryFile2Name, std::ios::out | std::ios::binary);
        int content;
        binFile.read(reinterpret_cast<char*>(&content), sizeof(int));

        if (content != binaryFile1Content) {
            throw FileSyncException("Content of downloaded binary file is not as expected",
                __FILE__, __LINE__);            
        }
    }

    void UploadDownload::cleanUp() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file1RemotePath);
        client.deleteRemoteFile();     
    }

}