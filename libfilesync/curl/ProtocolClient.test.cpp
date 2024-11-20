#include <libfilesync/curl/ProtocolClient.test.hpp>
#include <libfilesync/curl/wrapper/EasyStub.test.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>

using namespace filesync;
using namespace filesync::curl;
using namespace filesync::utility;
using namespace filesync::utility::literals;

int main(int argc, char* argv[]) {

    unittest::ProtocolClient protocolClientTest;

    protocolClientTest.set_bad_host_name();

    protocolClientTest.set_input_filesize_to_0();
    protocolClientTest.set_input_filesize_to_1KB();
    protocolClientTest.set_input_filesize_to_2047MB();
    protocolClientTest.set_input_filesize_to_2GB();
    protocolClientTest.set_input_filesize_to_99TB();
    protocolClientTest.set_input_filesize_to_n1KB();

    protocolClientTest.set_valid_remote_file();
    protocolClientTest.set_remote_file_with_special_chars();
    protocolClientTest.set_non_existing_local_file_for_upload();

    protocolClientTest.download_uninitialized_easy_stubbed();
    protocolClientTest.download_with_uninitialized_local_path_easy_stubbed();

    protocolClientTest.upload_uninitialized_easy_stubbed();
    protocolClientTest.upload_with_uninitialized_local_path_easy_stubbed();

    Logger::getInstance().log(LogDomain::TestResult, "ProtocolClient: passed", __FILE__, __LINE__);
    return 0;

}

namespace filesync::curl::unittest {

    std::string dummyServerAddress = "dummy";

    void ProtocolClient::set_bad_host_name() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_bad_host_name()", __FILE__, __LINE__);
        bool curlExceptionThrown = false;
        try {
            ProtocolClientConcrete client("::dummy::",
                std::make_unique<wrapper::unittest::EasyStub>());
        } catch (Exception& e) {
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);   

    }

    void ProtocolClient::set_input_filesize_to_0() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_0()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setInputFileSize(0);
    }

    void ProtocolClient::set_input_filesize_to_1KB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_1KB()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setInputFileSize(1_KB);
    }

    void ProtocolClient::set_input_filesize_to_2047MB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_2047MB()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setInputFileSize(2047_MB);
    }

    void ProtocolClient::set_input_filesize_to_2GB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_2GB()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setInputFileSize(2_GB);
    }

    void ProtocolClient::set_input_filesize_to_99TB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_99TB()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setInputFileSize(99_TB);
    }

    void ProtocolClient::set_input_filesize_to_n1KB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_n1KB()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.setInputFileSize(-1_KB);
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);       
    }

    void ProtocolClient::set_valid_remote_file() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_valid_remote_file()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setRemoteFile("dir-path/file");
    }

    void ProtocolClient::set_remote_file_with_special_chars() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_remote_file_with_special_chars()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        client.setRemoteFile("!./%/:kla");
    }

    void ProtocolClient::set_non_existing_local_file_for_upload() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_non_existing_local_file_for_upload()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.setLocalFileForUpload("this_file_does_not_exist.txt");
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);      
    }

    void ProtocolClient::download_uninitialized_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running download_uninitialized_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.download();
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);           
    }

    void ProtocolClient::download_with_uninitialized_local_path_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running download_with_uninitialized_local_path_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.setRemoteFile("remotefile");
            client.download();
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);           
    }

    void ProtocolClient::upload_uninitialized_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running upload_uninitialized_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.upload();
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);        
    }

    void ProtocolClient::upload_with_uninitialized_local_path_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running upload_with_uninitialized_local_path_easy_stubbed()",
            __FILE__, __LINE__);
            
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unittest::EasyStub>());
        bool curlExceptionThrown = false;
        try {
            client.setRemoteFile("remotefile");
            client.upload();
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);        
    }

}