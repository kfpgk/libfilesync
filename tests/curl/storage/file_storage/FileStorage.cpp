#include <tests/curl/storage/file_storage/FileStorage.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <filesystem>
#include <fstream>

namespace filesync::integration_test::curl::storage::file_storage {

    FileStorage::FileStorage(const std::string& testName) :
        IntegrationTest(testName),
        inputFile1Name{"file1"},
        inputFile1Content{"file 1 content"},
        inputFile2Name{"file2"},
        inputFile2Content{"file 2 content"} {

        TestCase swapTestCase {
            .name = "Swap two buffers",
            .perform = std::bind(&FileStorage::performSwap, this),
            .evaluate = std::bind(&FileStorage::evaluateSwap, this)
        };
        addTestCase(swapTestCase);

        TestCase copyConstruction {
            .name = "Test copy construction",
            .perform = std::bind(&FileStorage::performCopyConstruction, this),
            .evaluate = std::bind(&FileStorage::evaluateCopyConstruction, this)
        };
        addTestCase(copyConstruction);

        TestCase moveConstruction {
            .name = "Test move construction",
            .perform = std::bind(&FileStorage::performMoveConstruction, this),
            .evaluate = std::bind(&FileStorage::evaluateMoveConstruction, this)
        };
        addTestCase(moveConstruction);

        TestCase copyAssignment {
            .name = "Test copy assignment",
            .perform = std::bind(&FileStorage::performCopyAssignment, this),
            .evaluate = std::bind(&FileStorage::evaluateCopyAssignment, this)
        };
        addTestCase(copyAssignment);

        TestCase moveAssignment {
            .name = "Test move assignment",
            .perform = std::bind(&FileStorage::performMoveAssignment, this),
            .evaluate = std::bind(&FileStorage::evaluateMoveAssignment, this)
        };
        addTestCase(moveAssignment);

    }

    void FileStorage::setup() {
        std::ofstream file1(inputFile1Name);
        file1 << inputFile1Content << std::endl;
        std::ofstream file2(inputFile2Name);
        file2 << inputFile2Content << std::endl;
    }

    void FileStorage::performSwap() {
        namespace filesync = filesync::curl::storage;
        filesync::FileStorage storage1(inputFile1Name);
        filesync::FileStorage storage2(inputFile2Name);

        using std::swap;
        swap(storage1, storage2);

        resultOfPath1 = storage1.getPath() == inputFile2Name;
        resultOfPath2 = storage2.getPath() == inputFile1Name;
    }

    void FileStorage::evaluateSwap() {
        if (!resultOfPath1 || !resultOfPath2) {
            throw FileSyncException("File paths not as expected after swapping",
                __FILE__, __LINE__);          
        }     
    }

    void FileStorage::performCopyConstruction() {
        namespace filesync = filesync::curl::storage;
        filesync::FileStorage storage1(inputFile1Name);

        filesync::FileStorage storage2(storage1);

        std::filesystem::path file1FsPath(inputFile1Name);
        resultOfPath1 = storage1.getPath() == inputFile1Name;
        resultOfPath2 = 
            storage2.getPath() == 
                std::string(file1FsPath.stem()) + 
                "_copy" + 
                std::string(file1FsPath.extension());
        
        resultOfIsEqual = data::areEqual(storage1.getPath(), storage2.getPath());
    }

    void FileStorage::evaluateCopyConstruction() {
        if (!resultOfPath1) {
            throw FileSyncException("File path of constructed from object not as expected.",
                __FILE__, __LINE__);          
        }     
        if (!resultOfPath2) {
            throw FileSyncException("File path of constructed object not as expected.",
                __FILE__, __LINE__);          
        }        
        if (!resultOfIsEqual) {
            throw FileSyncException("File contents are not equal after copy construction",
                __FILE__, __LINE__);          
        }
    }

    void FileStorage::performMoveConstruction() {
        namespace filesync = filesync::curl::storage;
        filesync::FileStorage storage1(inputFile1Name);

        filesync::FileStorage storage2(std::move(storage1));

        resultOfPath1 = storage1.getPath() == "";
        resultOfPath2 = storage2.getPath() == inputFile1Name;
    }

    void FileStorage::evaluateMoveConstruction() {
        if (!resultOfPath1 || !resultOfPath2) {
            throw FileSyncException("File paths not as expected after move construction",
                __FILE__, __LINE__);          
        }     
    }

    void FileStorage::performCopyAssignment() {
        namespace filesync = filesync::curl::storage;
        filesync::FileStorage storage1(inputFile1Name);

        filesync::FileStorage storage2(inputFile2Name);

        storage2 = storage1;

        std::filesystem::path file1FsPath(inputFile1Name);
        resultOfPath1 = storage1.getPath() == inputFile1Name;
        resultOfPath2 = 
            storage2.getPath() == 
                std::string(file1FsPath.stem()) + 
                "_copy" + 
                std::string(file1FsPath.extension());
        
        resultOfIsEqual = data::areEqual(storage1.getPath(), storage2.getPath());
    }

    void FileStorage::evaluateCopyAssignment() {
        if (!resultOfPath1) {
            throw FileSyncException("File path of constructed from object not as expected.",
                __FILE__, __LINE__);          
        }     
        if (!resultOfPath2) {
            throw FileSyncException("File path of constructed object not as expected.",
                __FILE__, __LINE__);          
        }         
        if (!resultOfIsEqual) {
            throw FileSyncException("File contents are not equal after copy assignment",
                __FILE__, __LINE__);          
        }
    }

    void FileStorage::performMoveAssignment() {
        namespace filesync = filesync::curl::storage;
        filesync::FileStorage storage1(inputFile1Name);

        filesync::FileStorage storage2(inputFile2Name);

        storage2 = std::move(storage1);

        resultOfPath1 = storage1.getPath() == "";
        resultOfPath2 = storage2.getPath() == inputFile1Name;
    }

    void FileStorage::evaluateMoveAssignment() {
        if (!resultOfPath1 || !resultOfPath2) {
            throw FileSyncException("File paths not as expected after move construction",
                __FILE__, __LINE__);          
        }     
    }

}