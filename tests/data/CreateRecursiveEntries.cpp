#include <tests/data/CreateRecursiveEntries.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/data/DataException.hpp>

#include <stack>
#include <cassert>

namespace filesync::integrationtest::data {

    CreateRecursiveEntries::CreateRecursiveEntries(const std::string& testName) :
        IntegrationTest(testName) {

    }

    void CreateRecursiveEntries::setup() {
        try {
            std::filesystem::create_directory("dir1");
            filesync::data::createFile("dir1/file1");
            filesync::data::createFile("dir1/file2");
            std::filesystem::create_directory("dir1/dir2");
            filesync::data::createFile("dir1/dir2/file3");
            filesync::data::createFile("dir1/dir2/file4");
            filesync::data::createFile("dir1/dir2/file1");
        } catch(FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }

    }

    void CreateRecursiveEntries::perform() {
        entry = filesync::data::createEntryRecursively("dir1");
    }

    void CreateRecursiveEntries::evaluate() {
        entry->print();

        std::stack<std::string> entryStack;
        entryStack.push("dir1");
        entryStack.push("dir1/file1");
        entryStack.push("dir1/file2");
        entryStack.push("dir1/dir2");
        entryStack.push("dir1/dir2/file3");
        entryStack.push("dir1/dir2/file4");
        entryStack.push("dir1/dir2/file1");

        while (!entryStack.empty()) {
            std::string singleEntry = entryStack.top();
            entryStack.pop();
            if (!entry->getEntry(singleEntry)) {
                throw DataException("'" + singleEntry + "' not part of entry data structure.",
                    __FILE__, __LINE__);
            }            
        }
    }

}