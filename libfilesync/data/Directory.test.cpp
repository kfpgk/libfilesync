#include <libfilesync/data/Directory.test.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <string>
#include <memory>
#include <cassert>

using namespace filesync;
using namespace filesync::unittest;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    DirectoryTest directoryTest;

    directoryTest.add_file();
    directoryTest.add_directory();
    directoryTest.remove_file();
    directoryTest.remove_directory();

    directoryTest.set_remotes_with_relative_local_paths();
    directoryTest.set_remotes_with_deep_hierarchy();
    directoryTest.set_remotes_with_gap_hierarchy();

    Logger::getInstance().log(LogDomain::TestResult, "Class Directory: passed", __FILE__, __LINE__);
    return 0;

}

namespace filesync {
namespace unittest {

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

    void DirectoryTest::set_remotes_with_relative_local_paths() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running set_remotes_with_relative_local_paths()", __FILE__, __LINE__);
        
        std::string localDirPath("testdir");
        std::string localFilePath("testdir/blub");
        std::string remoteDirPath("remotedir");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<File> file = std::make_shared<File>(localFilePath);
        dir->addEntry(file);

        dir->setRemote(remoteDirPath);

        assert("remotedir" == dir->getRemotePath());
        assert("remotedir/blub" == dir->getEntry(localFilePath)->getRemotePath());

        Logger::getInstance().log(LogDomain::TestResult,
            "set_remotes_with_relative_local_paths() passed");
    }

    void DirectoryTest::set_remotes_with_deep_hierarchy() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running set_remotes_with_deep_hierarchy()", __FILE__, __LINE__);
        
        std::string localDirPath("testdir");
        std::string localSubDirPath("testdir/subdir");
        std::string localFilePath("testdir/subdir/blub");
        std::string remoteDirPath("remotedir");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<Directory> subdir = std::make_shared<Directory>(localSubDirPath);
        std::shared_ptr<File> file = std::make_shared<File>(localFilePath);
        subdir->addEntry(file);
        dir->addEntry(subdir);

        dir->setRemote(remoteDirPath);

        assert("remotedir" == dir->getRemotePath());
        assert("remotedir/subdir" == dir->getEntry(localSubDirPath)->getRemotePath());
        assert("remotedir/subdir/blub" == dir->getEntry(localFilePath)->getRemotePath());

        Logger::getInstance().log(LogDomain::TestResult,
            "set_remotes_with_deep_hierarchy() passed");
    }

    /**
     * The remote side will reflect the composite data
     * structure and not the local file system structure.
     * Subdirectories that are not part of the composite
     * data structure are not part of the remote structure.
     */
    void DirectoryTest::set_remotes_with_gap_hierarchy() {
        Logger::getInstance().log(LogDomain::TestResult,
            "Running set_remotes_with_gap_hierarchy()", __FILE__, __LINE__);
        
        std::string localDirPath("testdir");
        std::string localFilePath("testdir/subdir/blub");
        std::string remoteDirPath("remotedir");

        std::shared_ptr<Directory> dir = std::make_shared<Directory>(localDirPath);
        std::shared_ptr<File> file = std::make_shared<File>(localFilePath);
        dir->addEntry(file);

        dir->setRemote(remoteDirPath);

        assert("remotedir" == dir->getRemotePath());
        assert("remotedir/blub" == dir->getEntry(localFilePath)->getRemotePath());

        Logger::getInstance().log(LogDomain::TestResult,
            "set_remotes_with_gap_hierarchy() passed");
    }

}
}