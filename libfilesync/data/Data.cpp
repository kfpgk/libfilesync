#include <libfilesync/data/Data.hpp>
#include <libfilesync/data/DataException.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <string>
#include <fstream>

namespace filesync {
namespace data {

    void createFile(const std::filesystem::path& path) {
        if (std::filesystem::exists(path)) {
            throw DataException("Cannot create file '" + path.string() \
                + "' since it already exists.");
        }
        std::ofstream newFile(path);
        if (!newFile.is_open()) {
            throw DataException("Failed to create file '" + path.string() + "'");
        }
    }

    /**
     * @brief Build an Entry data structure
     * based on file system path
     * 
     * Create composite Entry object including
     * all files and directories in "path" argument.
     * 
     * If "path" is a single file, return an Entry
     * object that represents that single file.
     */
    std::unique_ptr<Entry> createEntryRecursively(
        const std::filesystem::path& path) {

        DEBUG("Parsing file system path '" << path.string() << "' and build up Entry data structure.");
        std::unique_ptr<Entry> entry;    
        if (std::filesystem::is_directory(path)) {
            DEBUG("'" << path.string()  << "' is a directory.");
            entry = std::make_unique<Directory>(path);
            using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
            for (const auto& nextEntry : recursive_directory_iterator(path)) {
                DEBUG("Adding '" << nextEntry.path() << "'");
                std::unique_ptr<Entry> newEntry;
                if (nextEntry.is_directory()) {
                    newEntry = std::make_unique<Directory>(nextEntry);                   
                } else if(nextEntry.is_regular_file()) {
                    newEntry = std::make_unique<File>(nextEntry);                    
                } else {
                    throw DataException(std::string("Irregular file '" + nextEntry.path().string()\
                        + "'. Cannot create entry from this."));
                }
                Entry* parent = entry->getEntry(nextEntry.path().parent_path());
                if (parent) {
                    Directory* dirParent = static_cast<Directory*>(parent);
                    if (dirParent) {
                        dirParent->addEntry(std::move(newEntry)); 
                    }
                } else {
                    throw DataException(std::string("Cannot add file '" + nextEntry.path().string()\
                        + "' with unknown parent directory '" + nextEntry.path().parent_path().string() + "'."));
                }                         
            }      
        } else if (std::filesystem::is_regular_file(path)) {
            DEBUG("'" << path.string()  << "' is a regular file.");
            entry = std::make_unique<File>(path);
        } else {
            throw DataException(std::string("'" + path.string() + "' not found"));
        }
        return std::move(entry);
    }

}
}