#include <libfilesync/data/Directory.test.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <string>
#include <memory>
#include <cassert>

using namespace filesync;
using namespace filesync::data::unit_test;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    DirectoryTest directoryTest;

    directoryTest.add_file();
    directoryTest.add_directory();
    directoryTest.remove_file();
    directoryTest.remove_directory();

    Logger::getInstance().log(LogDomain::TestResult, "Class data::Directory: passed", __FILE__, __LINE__);
    return 0;

}

namespace filesync::data::unit_test {

    void DirectoryTest::add_file() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running add_file()", __FILE__, __LINE__);

        std::string localDirPath("testdir");
        std::string localFilePath("testdir/blub");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<File> file = std::make_shared<File>(localFilePath);

        dir->addEntry(file);

        Entry* entry = dir->getEntry(localFilePath);

        assert(nullptr != entry);
        assert(localFilePath == dir->getEntry(localFilePath)->getPath());

        Logger::getInstance().log(LogDomain::TestResult,
            "add_file() passed");
    }

    void DirectoryTest::add_directory() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running add_directory()", __FILE__, __LINE__);

        std::string localDirPath("testdir");
        std::string localSubdirPath("testdir/blub");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<Directory> subdir = std::make_shared<Directory>(localSubdirPath);

        dir->addEntry(subdir);

        Entry* entry = dir->getEntry(localSubdirPath);

        assert(nullptr != entry);
        assert(localSubdirPath == dir->getEntry(localSubdirPath)->getPath());

        Logger::getInstance().log(LogDomain::TestResult,
            "add_directory() passed");        
    }

    void DirectoryTest::remove_file() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running remove_file()", __FILE__, __LINE__);

        std::string localDirPath("testdir");
        std::string localFilePath("testdir/blub");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<File> file = std::make_shared<File>(localFilePath);

        dir->addEntry(file);
        dir->removeEntry(localFilePath);

        Entry* entry = dir->getEntry(localFilePath);

        assert(nullptr == entry);

        Logger::getInstance().log(LogDomain::TestResult,
            "remove_file() passed");
    }

    void DirectoryTest::remove_directory() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running remove_directory()", __FILE__, __LINE__);

        std::string localDirPath("testdir");
        std::string localSubdirPath("testdir/blub");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<Directory> subdir = std::make_shared<Directory>(localSubdirPath);

        dir->addEntry(subdir);
        dir->removeEntry(localSubdirPath);

        Entry* entry = dir->getEntry(localSubdirPath);

        assert(nullptr == entry);

        Logger::getInstance().log(LogDomain::TestResult,
            "remove_directory() passed");      
    }

}