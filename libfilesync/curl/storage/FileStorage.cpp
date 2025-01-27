#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/option/Generic.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cstring>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace filesync::curl::storage {

    FileStorage::FileStorage(const std::filesystem::path& path) :
        path{path} {

    }

    FileStorage::FileStorage(const FileStorage& rhs) {
        path = 
            std::string(rhs.path.stem()) + 
            "_copy" + 
            std::string(rhs.path.extension());
        if (std::filesystem::is_regular_file(rhs.path)) {
            std::filesystem::copy_file(
                rhs.path,
                this->path,
                std::filesystem::copy_options::update_existing);
        }
    }

    FileStorage::FileStorage(FileStorage&& rhs) {
        using std::swap;
        swap(*this, rhs);
    }

    FileStorage& FileStorage::operator=(FileStorage rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    FileStorage::~FileStorage() {
        if (filePointer) {
            std::fflush(filePointer);
            std::fclose(filePointer);
        }
    }

    void swap(FileStorage& lhs, FileStorage& rhs) {
        lhs.path = std::exchange(rhs.path, lhs.path);
        lhs.filePointer = std::exchange(rhs.filePointer, lhs.filePointer);
    }

    std::filesystem::path FileStorage::getPath() const {
        return path;
    }

    void FileStorage::setupRead(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_READFUNCTION, &fileStorageReadCallback)->set();
        setFile(optionFactory, FileAccessType::read);
    }

    void FileStorage::setupWrite(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_WRITEFUNCTION, &fileStorageWriteCallback)->set();
        setFile(optionFactory, FileAccessType::write);
    }

    void FileStorage::flush() {
        if (filePointer) {
            std::fflush(filePointer);
        }
    }

    void FileStorage::setFile(const option::Factory& optionFactory,
        FileAccessType fileAccess) {

        setFilePointer(path, fileAccess);
        std::unique_ptr<option::Option> option;
        if (fileAccess == FileAccessType::read) {
            option = optionFactory.createGeneric(CURLOPT_READDATA, getFilePointer());
        } else if(fileAccess == FileAccessType::write) {
            option = optionFactory.createGeneric(CURLOPT_WRITEDATA, getFilePointer());
        }
        option->set();
    }

    std::FILE* FileStorage::getFilePointer(bool throwIfNull) const {
        if (!filePointer && throwIfNull) {
            throw Exception("filePointer not set '", \
                __FILE__, __LINE__);               
        }
        return filePointer;
    }

    void FileStorage::setFilePointer(const std::filesystem::path& path,
        FileAccessType fileAccess) {

        std::string fileAccessFlags = "r+b";
        if (fileAccess == FileAccessType::write) {
            fileAccessFlags = "w+b";
        }    
        if (filePointer) {
            std::fclose(filePointer);
        }
        filePointer = nullptr;
        filePointer = std::fopen(path.string().c_str(), fileAccessFlags.c_str());
        if (!filePointer) {
            throw Exception(std::string("fopen() failed on '" \
                + path.string() + "' with '" + std::strerror(errno) \
                + "'"), __FILE__, __LINE__);              
        }
    }

    void FileStorage::setInputFileSizeFromFile(const option::Factory& optionFactory,
        const std::filesystem::path& path) {
        std::uintmax_t fileSize = std::filesystem::file_size(path);
        if (fileSize < 0) {
            throw Exception(std::string("Could not get file size of '" \
                + path.string()), __FILE__, __LINE__);  
        }
        setInputFileSize(optionFactory, fileSize);
    }

    void FileStorage::setInputFileSize(const option::Factory& optionFactory,
        std::uintmax_t size) {
        try {
            using namespace utility::literals;
            std::unique_ptr<option::Option> option;
            if (size > 2_GB) {
                option = optionFactory.createGeneric(CURLOPT_INFILESIZE_LARGE, size);
            } else {
                option = optionFactory.createGeneric(CURLOPT_INFILESIZE, size);
            }
            option->set();
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    /**
     * @brief Write callback function which writes incoming
     * data to a file.
     * 
     * @param[in] target Target file which the recieved data
     * is written to. If it is NULL, we discard the received
     * data.
     */
    extern "C" size_t fileStorageWriteCallback(char* contents,
        size_t size, size_t count, FILE* target) {

        if (target) {
            return std::fwrite(contents, size, count, target);
        } else {
            return count;
        }
    }

    /**
     * @brief Read callback function which reads outgoing
     * data from a file.
     * 
     * @param[in] contents Source file which the outgoing data
     * is read from.
     */
    extern "C" size_t fileStorageReadCallback(char* buffer,
        size_t size, size_t count, FILE* contents) {

        return std::fread(buffer, size, count, contents);  
    }

}