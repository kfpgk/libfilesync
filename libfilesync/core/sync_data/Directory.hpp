#ifndef LIBFILESYNC_CORE_SYNC_DATA_DIRECTORY_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_DIRECTORY_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/observer/Observer.hpp>

#include <filesystem>
#include <deque>
#include <memory>

namespace filesync::core::sync_data {

    class Directory : public Entry, public filesync::data::DirectoryBase<Entry> {

        public:
            explicit Directory(const std::filesystem::path& path);

        private:
            void doPrint() const override; 
            void doSetRemoteEntry(const std::filesystem::path& path) override;
            void doSetSyncInProgress() override;
            void doResetSyncInProgress() override;
            [[nodiscard]] bool doGetSyncInProgress() const override;
            [[nodiscard]] bool doLocalDifferentThanPrev() const override;
            [[nodiscard]] bool doRemoteDifferentThanPrev() const override;
            void doSetPrevious() override;
            void doWriteRemoteBufferToLocal() override;

    };

}

#endif