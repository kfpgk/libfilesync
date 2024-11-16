#include <tests/core/sync_data/file_buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/FileBuffer.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <fstream>
#include <cstdio>

namespace filesync::integrationtest::core::sync_data::file_buffer {

    FileBuffer::FileBuffer(const std::string& testName) :
        IntegrationTest(testName),
        file1Name{"file1"},
        file1Content{"file 1 content"},
        file2Name{"file2"},
        file2Content{"file 2 content"} {

    }

    void FileBuffer::setup() {
        std::ofstream file1(file1Name);
        file1 << file1Content << std::endl;
    }

    void FileBuffer::perform() {
        filesync::core::sync_data::FileBuffer buffer;
        std::ifstream readFile(file1Name);
        buffer.store(readFile);

        std::ofstream writeFile(file2Name);
        buffer.writeContentTo(writeFile);
    }

    void FileBuffer::evaluate() {
        if (!filesync::data::areEqual(file1Name, file2Name)) {
            throw FileSyncException("Data read from buffer is not equal to data that was stored"\
                " to the buffer", __FILE__, __LINE__);          
        }
    }

}