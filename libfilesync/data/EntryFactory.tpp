#include <libfilesync/data/EntryFactory.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/data/Directory.hpp>

namespace filesync::data {

    template<typename Entry, typename File, typename Directory>
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
                    throw Exception(std::string("Irregular file '" + nextEntry.path().string()\
                        + "'. Cannot create entry from this."));
                }
                Entry* parent = entry->getEntry(nextEntry.path().parent_path());
                if (parent) {
                    Directory* dirParent = static_cast<Directory*>(parent);
                    if (dirParent) {
                        dirParent->addEntry(std::move(newEntry)); 
                    }
                } else {
                    throw Exception(std::string("Cannot add file '" + nextEntry.path().string()\
                        + "' with unknown parent directory '" + nextEntry.path().parent_path().string() + "'."));
                }                         
            }      
        } else if (std::filesystem::is_regular_file(path)) {
            DEBUG("'" << path.string()  << "' is a regular file.");
            entry = std::make_unique<File>(path);
        } else {
            throw Exception(std::string("'" + path.string() + "' not found"));
        }
        return std::move(entry);
    }

}