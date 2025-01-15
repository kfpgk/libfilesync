#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/GetHandle.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/IsEqualTo.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/Store.hpp>
#include <libfilesync/core/sync_data/buffer/visitor/WriteContentTo.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <fstream>
#include <sstream>
#include <type_traits>
#include <utility>

using namespace filesync::utility;

namespace filesync::core::sync_data {

    Entry::Entry(const std::filesystem::path& path) :
        EntryBase{path},
        bufferForRemote{std::make_unique<buffer::Buffer>(
            std::in_place_type<buffer::FileBuffer>)},
        bufferForPrevious{std::make_unique<buffer::Buffer>(
            std::in_place_type<buffer::FileBuffer>)},
        remoteEntry{std::make_unique<RemoteEntry>(path)} {

    }

    Entry::Entry(const std::filesystem::path& path,
            const buffer::Buffer& bufferForRemote,
            const buffer::Buffer& bufferForPrevious) :
        EntryBase{path},
        remoteEntry{std::make_unique<RemoteEntry>(path)} {

        setBuffers(bufferForRemote, bufferForPrevious);

    }

    void Entry::setBuffers(
        const buffer::Buffer& bufferForRemote,
        const buffer::Buffer& bufferForPrevious) {
    
        doSetBuffers(bufferForRemote, bufferForPrevious);
    }

    void Entry::doSetBuffers(
        const buffer::Buffer& bufferForRemote,
        const buffer::Buffer& bufferForPrevious) {


        if (bufferTypeSupportsRemoteBuffer(bufferForRemote)) {
            
            this->bufferForRemote = std::make_unique<buffer::Buffer>(
                std::remove_reference<decltype(bufferForRemote)>::type{});
        } else {
            throw data::Exception("This buffer type cannot be used for the buffer "\
                "that holds remote data.",
                __FILE__, __LINE__);
        }

        if (bufferTypeSupportsPreviousBuffer(bufferForPrevious)) {
            
            this->bufferForPrevious = std::make_unique<buffer::Buffer>(
                std::remove_reference<decltype(bufferForPrevious)>::type{});
        } else {
            throw data::Exception("This buffer type cannot be used for the buffer "\
                "holding previous data.",
                __FILE__, __LINE__);
        }        

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
            std::string(getPath().string() + " <-> <server>/<rootPath>/" +
                remoteEntry->getPath()));        
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

        std::visit(buffer::visitor::Store{file}, *bufferForPrevious);
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
        return !std::visit(buffer::visitor::IsEqualTo{file}, *bufferForPrevious);
    }

    protocol::HandleOrFilePath Entry::getRemoteBufferHandle() {

        if (auto buffer = std::get_if<buffer::FileBuffer>(&*bufferForRemote)) {
            return buffer->getFilePath();
        } else if (auto buffer = std::get_if<buffer::ProtocolMemoryBuffer>(&*bufferForRemote)) {
            return buffer->getHandle();
        } else {
            throw data::Exception("This buffer type cannot be used for the remote buffer.",
                __FILE__, __LINE__);
        }
    }

    bool Entry::remoteDifferentThanPrev() {
        return doRemoteDifferentThanPrev();
    }

    bool Entry::doRemoteDifferentThanPrev() {
        std::stringstream stream;
        std::visit(buffer::visitor::WriteContentTo{stream}, *bufferForRemote);
        return !std::visit(buffer::visitor::IsEqualTo{stream}, *bufferForPrevious);
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

        std::visit(buffer::visitor::WriteContentTo{file}, *bufferForRemote);
    }

    bool Entry::bufferTypeSupportsRemoteBuffer(const buffer::Buffer& buffer) const {
        if (std::holds_alternative<buffer::FileBuffer>(buffer) ||
            std::holds_alternative<buffer::ProtocolMemoryBuffer>(buffer)) {
            
            return true;
        }
        return false;
    }

    bool Entry::bufferTypeSupportsPreviousBuffer(const buffer::Buffer& buffer) const {
        if (std::holds_alternative<buffer::FileBuffer>(buffer) ||
            std::holds_alternative<buffer::CharArrayMemoryBuffer>(buffer)) {
            
            return true;
        }
        return false;
    }

}