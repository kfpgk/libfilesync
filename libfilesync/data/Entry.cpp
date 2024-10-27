#include <libfilesync/data/Entry.hpp>
#include <libfilesync/data/DataException.hpp>
#include <libfilesync/utility/TimeFormat.hpp>
#include <libfilesync/utility/Logger.hpp>

using namespace filesync::utility;

namespace filesync {

    Entry::Entry(const std::filesystem::path& path) :
        path{path} {

        this->path = this->path.make_preferred();
        if (validate()) {
            lastWriteTime = std::filesystem::last_write_time(path);
        } else {
            lastWriteTime = std::chrono::file_clock::now();
        }           
    }

    std::filesystem::path Entry::getPath() const {
        return path;
    }

    std::filesystem::path Entry::getAbsolutePath() const {
        return std::filesystem::absolute(path);
    }

    std::string Entry::notFoundMessage() const {
        return std::string("'" + path.string() + "': not found");  
    }

    Entry* Entry::getEntry(const std::filesystem::path& path) {
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
    Entry* Entry::doGetEntry(const std::filesystem::path& path) {

        [[__maybe_unused__]] std::error_code ec;
        if (std::filesystem::equivalent(getPath(), path, ec) ||
            getAbsolutePath() == std::filesystem::absolute(path)) {
            return this;
        } else {
            return nullptr;
        }      
    }

    void Entry::setRemote(const std::string& path) {
        doSetRemote(path);
    }

    void Entry::doSetRemote(const std::string& path) {
        remote = std::make_optional<RemoteEntry>(path);
    }

    bool Entry::hasRemote() const {
        return remote != std::nullopt;
    }

    std::string Entry::getRemotePath() const {
        if (!remote) {
            std::string errorMessage = std::string("No remote set for '" + path.string() + "'");
            throw DataException(errorMessage, __FILE__, __LINE__);
        }
        return remote.value().getPath();
    }

    bool Entry::validate() const {
        return doValidate();
    }

    bool Entry::doValidate() const {
        bool valid = std::filesystem::exists(path);
        if (!valid) {
            Logger::getInstance().logOnce(LogDomain::Warning, notFoundMessage());
        }
        return valid;
    }

    void Entry::performOnChange() {
        doPerformOnChange();
    }

    void Entry::doPerformOnChange() {
        if (hasChanged()) {
            onChange();
        }
    }

    void Entry::print() const {
        doPrint();
    }

    void Entry::doPrint() const {
        std::string remotePath = "undefined";
        if (remote) {
            remotePath = getRemotePath();
        }
        Logger::getInstance().log(LogDomain::Info,
            std::string(getPath().string() + " <-> <server>/<rootPath>/" + remotePath));
    }

    bool Entry::hasChanged() const {
        bool changed = false;
        if (validate()) {
            using namespace std::filesystem;
            file_time_type newLastWriteTime = last_write_time(path);
            changed = lastWriteTime != newLastWriteTime; 
        }
        return changed;
    }

    void Entry::resetChanged() {
        if (validate()) {
            using namespace std::filesystem;
            file_time_type newLastWriteTime = last_write_time(path);
            lastWriteTime = newLastWriteTime;
        }        
    }

    void Entry::onChange() {
        Logger::getInstance().logFileWriteAccess(LogDomain::Info, getPath(), lastWriteTime);
        notify();
        resetChanged();
    }

}