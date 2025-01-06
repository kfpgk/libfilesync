#include <tests/curl/ftp/GetFileSize.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <filesystem>
#include <fstream>

namespace filesync::integration_test::curl::ftp {

    constexpr auto separator = std::filesystem::path::preferred_separator;

    GetFileSize::GetFileSize(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer},
            file1Name{"file1"},
            file1RemotePath{pathOnServer + separator + file1Name},
            file1Content{"file1 content"},
            binaryFile1Name{"file1.bin"},
            binaryFile1RemotePath{pathOnServer + separator + binaryFile1Name},
            binaryFile1Content{42} {

    }

    void GetFileSize::setup() {
        std::ofstream localFile(file1Name);
        if (!localFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        localFile << file1Content << std::endl;
        localFile.close();

        std::ofstream binFile(binaryFile1Name, std::ios::binary | std::ios::out);
        binFile.write(reinterpret_cast<char*>(&binaryFile1Content), sizeof(int));

        std::filesystem::resize_file(file1Name, file1Size);
        std::filesystem::resize_file(binaryFile1Name, binaryFile1Size);

        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file1RemotePath);
        client.setLocalFileForUpload(file1Name);
        client.upload();   

        client.setRemoteFile(binaryFile1RemotePath);
        client.setLocalFileForUpload(binaryFile1Name);
        client.upload();         
    }

    void GetFileSize::perform() {

        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file1RemotePath);
        file1RetrievedSize = client.getRemoteFileSize();

        client.setRemoteFile(binaryFile1RemotePath);
        binaryFile1RetrievedSize = client.getRemoteFileSize();

    }

    void GetFileSize::evaluate() {
        if (file1RetrievedSize != file1Size) {
            throw FileSyncException("Retrieved text file size did not match.",
                __FILE__, __LINE__);
        }
        if (binaryFile1RetrievedSize != binaryFile1Size) {
            throw FileSyncException("Retrieved binary file size did not match.",
                __FILE__, __LINE__);
        }
    }

    void GetFileSize::cleanUp() {
        filesync::curl::FtpClient client(server);

        client.setRemoteFile(file1RemotePath);
        client.deleteRemoteFile();

        client.setRemoteFile(binaryFile1RemotePath);
        client.deleteRemoteFile();   
    }

}