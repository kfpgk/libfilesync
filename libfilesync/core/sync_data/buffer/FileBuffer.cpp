#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/data/Exception.hpp>

#include <cstdio>
#include <fstream>

namespace filesync::core::sync_data::buffer {

    FileBuffer::FileBuffer() :
        filePath{std::tmpnam(nullptr)} {

    }

    const std::filesystem::path& FileBuffer::getFilePath() const {
        return filePath;
    }

    void FileBuffer::store(std::istream& in) {
        if (in.fail()) {
            throw data::Exception("Cannot store. Input stream is in fail state.",
                __FILE__, __LINE__);
        }
        std::ofstream buffer(filePath);
        in.seekg(std::ios::beg);
        buffer << in.rdbuf();            
    }

    std::ostream& FileBuffer::extractContentTo(std::ostream& out) {
        if (!std::filesystem::is_regular_file(filePath)) {
            return out;
        }
        std::ifstream buffer(filePath);
        if (!buffer.is_open()) {
            throw data::Exception("Could not open buffer file for reading.",
                __FILE__, __LINE__);
        }
        out << buffer.rdbuf();
        return out;
    }

    bool FileBuffer::isEqualTo(std::istream& in) const {

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