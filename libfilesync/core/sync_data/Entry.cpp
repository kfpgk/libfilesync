#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/GetHandle.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/IsEqualTo.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/Store.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/WriteContentTo.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <fstream>
#include <utility>

using namespace filesync::utility;

namespace filesync::core::sync_data {

    Entry::Entry(const std::filesystem::path& path) :
        EntryBase{path},
        bufferForRemote{std::in_place_type<buffer::FileBuffer>},
        bufferForPrevious{std::in_place_type<buffer::FileBuffer>},
        remoteEntry{std::make_unique<RemoteEntry>(path)} {

    }

    std::filesystem::path Entry::getRemotePath() const {
        return remoteEntry->getPath();
    }

    void Entry::setRemoteEntry(const std::filesystem::path& path) {
        doSetRemoteEntry(path);
    }

    void Entry::doSetRemoteEntry(const std::filesystem::path& path) {
        remoteEntry = std::make_unique<RemoteEntry>(path);
    }

    void Entry::doPrint() const {
        Logger::getInstance().log(LogDomain::Info,
            std::string(getPath().string() + " <-> <server>/<rootPath>/" + remoteEntry->getPath()));        
    }

    void Entry::setSyncInProgress() {
        doSetSyncInProgress();
    }

    void Entry::doSetSyncInProgress() {
        syncInProgress = true;
    }

    void Entry::resetSyncInProgress() {
        doResetSyncInProgress();
    }

    void Entry::doResetSyncInProgress() {
        syncInProgress = false;
    }

    bool Entry::getSyncInProgress() const {
        return doGetSyncInProgress();
    }

    bool Entry::doGetSyncInProgress() const {
        return syncInProgress;
    }

    void Entry::setPrevious() {
        doSetPrevious();
    }

    void Entry::doSetPrevious() {
        std::ifstream file(getPath());
        if (!file.is_open()) {
            throw data::Exception("Cannot open file '" + getPath().string() + "' for storing.",
                __FILE__, __LINE__);
        }

        std::visit(buffer::visitor::Store{file}, bufferForPrevious);
    }

    bool Entry::localDifferentThanPrev() {
        return doLocalDifferentThanPrev();
    }

    bool Entry::doLocalDifferentThanPrev() {
        std::ifstream file(getPath());
        if (!file.is_open()) {
            throw data::Exception("Cannot open file '" + getPath().string() + "' for comparing.",
                __FILE__, __LINE__);
        }
        return !std::visit(buffer::visitor::IsEqualTo{file}, bufferForPrevious);
    }

    buffer::HandleType Entry::getRemoteBufferHandle() {
        return std::visit(buffer::visitor::GetHandle{}, bufferForRemote);
    }

    bool Entry::remoteDifferentThanPrev() {
        return doRemoteDifferentThanPrev();
    }

    bool Entry::doRemoteDifferentThanPrev() {
        std::ifstream file(std::get<std::filesystem::path>(
            std::visit(buffer::visitor::GetHandle{}, bufferForRemote)));

        if (!file.is_open()) {
            throw data::Exception("Cannot open file '" + getPath().string() + "' for comparing.",
                __FILE__, __LINE__);
        }
        
        return !std::visit(buffer::visitor::IsEqualTo{file}, bufferForPrevious);
    }

    void Entry::writeRemoteBufferToLocal() {
        doWriteRemoteBufferToLocal();
    }

    void Entry::doWriteRemoteBufferToLocal() {
        std::ofstream file(getPath());

        if (!file.is_open()) {
            throw data::Exception("Cannot open file '" + getPath().string() + "' to write to.",
                __FILE__, __LINE__);
        }

        std::visit(buffer::visitor::WriteContentTo{file}, bufferForRemote);
    }

}