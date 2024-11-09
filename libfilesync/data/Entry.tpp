#include <libfilesync/data/Data.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/utility/TimeFormat.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <chrono>

using namespace filesync::utility;

namespace filesync::data {

    template<typename T>
    EntryBase<T>::EntryBase(const std::filesystem::path& path) :
        path{path} {

        this->path = this->path.make_preferred();
        if (validate()) {
            lastWriteTime = std::filesystem::last_write_time(path);
        } else {
            lastWriteTime = std::chrono::file_clock::now();
        }           
    }

    template<typename T>
    std::filesystem::path EntryBase<T>::getPath() const {
        return path;
    }

    template<typename T>
    std::filesystem::path EntryBase<T>::getAbsolutePath() const {
        return std::filesystem::absolute(path);
    }

    template<typename T>
    std::string EntryBase<T>::notFoundMessage() const {
        return std::string("'" + path.string() + "': not found");  
    }

    template<typename T>
    T* EntryBase<T>::getEntry(const std::filesystem::path& path) {
        return doGetEntry(path);
    }

    /**
     * @brief Return Entry* if an entry corresponding to
     * path is part of this Entry structure.
     * 
     * std::filesystem::equivalent(..., error_code&) will 
     * return false if paths do not exist, so we recheck
     * manually.
     */
    template<typename T>
    T* EntryBase<T>::doGetEntry(const std::filesystem::path& path) {

        [[__maybe_unused__]] std::error_code ec;
        if (std::filesystem::equivalent(getPath(), path, ec) ||
            getAbsolutePath() == std::filesystem::absolute(path)) {
            return dynamic_cast<T*>(this);
        } else {
            return nullptr;
        }      
    }

    template<typename T>
    bool EntryBase<T>::validate() const {
        return doValidate();
    }

    template<typename T>
    bool EntryBase<T>::doValidate() const {
        bool valid = std::filesystem::exists(path);
        if (!valid) {
            Logger::getInstance().logOnce(LogDomain::Warning, notFoundMessage());
        }
        return valid;
    }

    template<typename T>
    bool EntryBase<T>::isEqualTo(const std::filesystem::path& path2) const {
        return data::areEqual(path, path2);
    }

    template<typename T>
    void EntryBase<T>::performOnChange() {
        doPerformOnChange();
    }

    template<typename T>
    void EntryBase<T>::doPerformOnChange() {
        if (hasChanged()) {
            onChange();
        }
    }

    template<typename T>
    void EntryBase<T>::print() const {
        doPrint();
    }

    template<typename T>
    void EntryBase<T>::doPrint() const {
        Logger::getInstance().log(LogDomain::Info, std::string(getPath().string()));
    }

    template<typename T>
    bool EntryBase<T>::hasChanged() const {
        bool changed = false;
        if (validate()) {
            using namespace std::filesystem;
            file_time_type newLastWriteTime = last_write_time(path);
            changed = lastWriteTime != newLastWriteTime; 
        }
        return changed;
    }

    template<typename T>
    void EntryBase<T>::resetChanged() {
        if (validate()) {
            using namespace std::filesystem;
            file_time_type newLastWriteTime = last_write_time(path);
            lastWriteTime = newLastWriteTime;
        }        
    }

    template<typename T>
    void EntryBase<T>::onChange() {
        Logger::getInstance().logFileWriteAccess(LogDomain::Info, getPath(), lastWriteTime);
        this->notify();
        resetChanged();
    }

}