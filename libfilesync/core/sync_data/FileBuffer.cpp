#include <libfilesync/core/sync_data/FileBuffer.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <fstream>
#include <filesystem>
#include <cstdio>
namespace filesync::core::sync_data {

    FileBuffer::FileBuffer() :
        filePath{std::tmpnam(nullptr)} {

    }

    void FileBuffer::doStore(std::istream& in) {
        std::ofstream buffer(filePath);
        in.seekg(std::ios::beg);
        buffer << in.rdbuf();            
    }

    bool FileBuffer::doWriteContentTo(std::ostream& out) {
        if (!std::filesystem::is_regular_file(filePath)) {
            return false;
        }
        std::ifstream buffer(filePath);
        if (!buffer.is_open()) {
            throw data::Exception("Could not open buffer file for reading.",
                __FILE__, __LINE__);
        }
        out << buffer.rdbuf();
        return true;
    }

    bool FileBuffer::doIsEqualTo(std::istream& in) {
        std::ifstream buffer(filePath, std::ifstream::binary | std::ifstream::ate);

        if (buffer.fail() || in.fail()) {
            return false;
        }

        buffer.seekg(0, std::ifstream::beg);
        in.seekg(0, std::istream::beg);
        return std::equal(
            std::istreambuf_iterator<char>(buffer.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(in.rdbuf()));

    }
    
}