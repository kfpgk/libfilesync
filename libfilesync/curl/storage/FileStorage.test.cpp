#include <libfilesync/curl/storage/FileStorage.test.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>

using namespace filesync::curl;
using namespace filesync::utility;
using namespace filesync::utility::literals;

int main(int argc, char* argv[]) {

    storage::unit_test::FileStorageTest fileStorageTest;

    fileStorageTest.set_input_filesize_to_0();
    fileStorageTest.set_input_filesize_to_1KB();
    fileStorageTest.set_input_filesize_to_2047MB();
    fileStorageTest.set_input_filesize_to_2GB();
    fileStorageTest.set_input_filesize_to_99TB();
    fileStorageTest.set_input_filesize_to_n1KB();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::storage::FileStorage: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::curl::storage::unit_test {

    void FileStorageTest::set_input_filesize_to_0() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_0()", __FILE__, __LINE__);
        
        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        fileStorage.setInputFileSize(factory, 0);
    }

    void FileStorageTest::set_input_filesize_to_1KB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_1KB()", __FILE__, __LINE__);

        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        fileStorage.setInputFileSize(factory, 1_KB);
    }

    void FileStorageTest::set_input_filesize_to_2047MB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_2047MB()", __FILE__, __LINE__);

        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        fileStorage.setInputFileSize(factory, 2047_MB);
    }

    void FileStorageTest::set_input_filesize_to_2GB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_2GB()", __FILE__, __LINE__);

        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        fileStorage.setInputFileSize(factory, 2_GB);
    }

    void FileStorageTest::set_input_filesize_to_99TB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_99TB()", __FILE__, __LINE__);

        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        fileStorage.setInputFileSize(factory, 99_TB);
    }

    void FileStorageTest::set_input_filesize_to_n1KB() {
        Logger::getInstance().log(LogDomain::TestResult, "Running set_filesize_to_n1KB()", __FILE__, __LINE__);

        wrapper::Easy easy;
        option::Factory factory(easy);
        storage::FileStorage fileStorage("any");
        bool curlExceptionThrown = false;
        try {
            fileStorage.setInputFileSize(factory, -1_KB);
        } catch (const Exception& e) {
            Logger::getInstance().log(LogDomain::Info,
                e.what(), __FILE__, __LINE__);
            curlExceptionThrown = true;
        }
        assert(true == curlExceptionThrown);       
    }


}