#include <libfilesync/data/Directory.hpp>

using namespace std;

namespace filesync {

    Directory::Directory(const std::filesystem::path& path) :
        Entry(path) {

    }

    void Directory::addEntry(std::shared_ptr<Entry> entry) {
        components.push_back(entry);
    }

    void Directory::removeEntry(const std::filesystem::path& path) {
        for(auto it = components.begin(); it != components.end();) {
            if (it->get()->getPath() == path) {
                it = components.erase(it);
            } else {
                it++;
            }            
        }
    }

    void Directory::registerObserver(Observer<Entry>& observer) {
        Entry::registerObserver(observer);
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->registerObserver(observer);
        }
    }

    void Directory::unregisterObserver(Observer<Entry>& observer) {
        Entry::unregisterObserver(observer);
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->unregisterObserver(observer);
        }
    }

    void Directory::notify() {
        Entry::notify();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->notify();
        }       
    }    

    Entry* Directory::doGetEntry(const std::filesystem::path& path) {
        if (Entry::doGetEntry(path)) return this;
        for(auto it = components.begin(); it != components.end(); it++) {
            Entry* entry = it->get()->getEntry(path);
            if (entry) {
                return entry;
            }           
        }
        return nullptr;
    }

    void Directory::doPerformOnChange() {
        Entry::doPerformOnChange();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->performOnChange();
        }
    }

    void Directory::doSetRemote(const std::string& path) {
        Entry::doSetRemote(path);
        for(auto it = components.begin(); it != components.end(); it++) {
            namespace fs = std::filesystem;
            fs::path childPath = it->get()->getPath();
            fs::path childRemote = path + fs::path::preferred_separator + childPath.filename().string();
            childRemote.make_preferred();
            it->get()->setRemote(childRemote.string());
        }
    }

    bool Directory::doValidate() const {
        bool valid = Entry::doValidate();
        for(auto it = components.begin(); it != components.end(); it++) {
            if (!it->get()->validate()) {
                valid = false;
            }              
        }
        return valid;    
    }

    void Directory::doPrint() const {
        Entry::doPrint();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->print();        
        }        
    }

}