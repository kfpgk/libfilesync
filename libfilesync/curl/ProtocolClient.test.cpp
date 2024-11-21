#include <libfilesync/curl/ProtocolClient.test.hpp>
#include <libfilesync/curl/wrapper/EasyStub.test.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>

using namespace filesync::curl;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    unit_test::ProtocolClientTest protocolClientTest;

    protocolClientTest.set_bad_host_name();

    protocolClientTest.set_valid_remote_file();
    protocolClientTest.set_remote_file_with_special_chars();
    protocolClientTest.set_non_existing_local_file_for_upload();

    protocolClientTest.download_uninitialized_easy_stubbed();
    protocolClientTest.download_with_uninitialized_local_path_easy_stubbed();

    protocolClientTest.upload_uninitialized_easy_stubbed();
    protocolClientTest.upload_with_uninitialized_local_path_easy_stubbed();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::ProtocolClient: passed", __FILE__, __LINE__);
    return 0;

}

namespace filesync::curl::unit_test {

    std::string dummyServerAddress = "dummy";

    void ProtocolClientTest::set_bad_host_name() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_bad_host_name()", __FILE__, __LINE__);
        bool curlExceptionThrown = false;
        try {
            ProtocolClientConcrete client("::dummy::",
                std::make_unique<wrapper::unit_test::EasyStub>());
        } catch (Exception& e) {
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);   

    }

    void ProtocolClientTest::set_valid_remote_file() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_valid_remote_file()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
        client.setRemoteFile("dir-path/file");
    }

    void ProtocolClientTest::set_remote_file_with_special_chars() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_remote_file_with_special_chars()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
        client.setRemoteFile("!./%/:kla");
    }

    void ProtocolClientTest::set_non_existing_local_file_for_upload() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_non_existing_local_file_for_upload()", __FILE__, __LINE__);
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
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

    void ProtocolClientTest::download_uninitialized_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running download_uninitialized_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
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

    void ProtocolClientTest::download_with_uninitialized_local_path_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running download_with_uninitialized_local_path_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
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

    void ProtocolClientTest::upload_uninitialized_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running upload_uninitialized_easy_stubbed()",
            __FILE__, __LINE__);

        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
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

    void ProtocolClientTest::upload_with_uninitialized_local_path_easy_stubbed() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running upload_with_uninitialized_local_path_easy_stubbed()",
            __FILE__, __LINE__);
            
        ProtocolClientConcrete client(dummyServerAddress,
            std::make_unique<wrapper::unit_test::EasyStub>());
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