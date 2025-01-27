#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <cstdio>
#include <fstream>
#include <utility>

namespace filesync::core::sync_data::buffer {

    FileBuffer::FileBuffer() :
        filePath{std::tmpnam(nullptr)} {

    }

    FileBuffer::FileBuffer(const FileBuffer& rhs) :
        filePath{std::tmpnam(nullptr)}  {
        
        if (std::filesystem::is_regular_file(rhs.filePath)) {
            std::filesystem::copy_file(
                rhs.filePath,
                this->filePath);
        }

    }

    FileBuffer::FileBuffer(FileBuffer&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    FileBuffer& FileBuffer::operator=(FileBuffer rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    FileBuffer::~FileBuffer() {
        if (std::filesystem::is_regular_file(filePath)) {           
            std::filesystem::remove(filePath);
        }
    }

    void swap(FileBuffer& lhs, FileBuffer& rhs) noexcept {
        lhs.filePath = std::exchange(rhs.filePath, lhs.filePath);
    }

    std::unique_ptr<FileBuffer> FileBuffer::cloneType() const {
        return std::make_unique<FileBuffer>();
    }

    bool FileBuffer::isEmpty() const {
        return !std::filesystem::is_regular_file(filePath);
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

        if (buffer.fail()) {
            DEBUG("The buffer's stream is in `fail` state. Return `false`.");
            return false;
        }

        if (in.fail()) {
            DEBUG("The input stream is in `fail` state. Return `false`.");
            return false;
        }

#       ifdef DEBUG_BUILD
            printContentOfBufferAnd(in);
#       endif

        resetInputStream(buffer);
        resetInputStream(in);

        bool isEqual = std::equal(
            std::istreambuf_iterator<char>(buffer.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(in.rdbuf()));

        if (!hasStreamReachedEnd(in)) {
            isEqual = false;
        }

        return isEqual;
    }

    bool FileBuffer::isEqualTo(FileBuffer& in) const {
        std::stringstream stream;
        in.extractContentTo(stream);
        return isEqualTo(stream);
    }

    bool FileBuffer::hasStreamReachedEnd(std::istream& in) const {
        return in.peek() == std::char_traits<char>::eof();
    }

    /**
     * @brief Resets the stream to beginning and clears
     * all flags.
     * 
     * clear() clears flags, including `eof` and `fail` 
     * flag which are being set e.g. by a `std::getline`
     * `while` loop. `seekg(...)` set position indication
     * to the beginning of the stream.
     */
    void FileBuffer::resetInputStream(std::istream& in) const {
        in.clear();
        in.seekg(0, std::istream::beg);
    }

    void FileBuffer::printContentOfBufferAnd(std::istream& in) const {
        std::ifstream buffer(filePath, std::ifstream::binary | std::ifstream::ate);

        resetInputStream(buffer);
        resetInputStream(in);
        
        std::string line;

        DEBUG("---------------");
        DEBUG("Buffer content: ");
        while (std::getline(buffer, line)) {
            DEBUG(line);
        }
        
        DEBUG("---------------");
        DEBUG("Compare to: ");
        while (std::getline(in, line)) {
            DEBUG(line);
        }
        DEBUG("---------------");    
    }
    
}