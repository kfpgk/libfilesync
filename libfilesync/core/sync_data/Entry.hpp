#ifndef LIBFILESYNC_CORE_SYNC_DATA_ENTRY_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_ENTRY_HPP

#include <libfilesync/data/Entry.hpp>
#include <libfilesync/observer/Observer.hpp>
#include <libfilesync/core/sync_data/RemoteEntry.hpp>
#include <libfilesync/core/sync_data/Buffer.hpp>

#include <filesystem>
#include <memory>

namespace filesync::core::sync_data {

    /**
     * @brief Syncable entry object
     * 
     * This object extends the Entry object by
     * a reference to a remote entry and cashing.
     * 
     * Patterns: 
     *  - Component object of the Composite pattern
     *  - Subject object of the Observer pattern
     *  - Non virtual interface
     *  - CRTP
     */
    class Entry : virtual public filesync::data::EntryBase<Entry> {

        public:
            explicit Entry(const std::filesystem::path& path);

            void setRemoteEntry(const std::filesystem::path& path);
            [[nodiscard]] std::filesystem::path getRemotePath() const;
            void setSyncInProgress();
            [[nodiscard]] bool getSyncInProgress() const;
            bool localChanged() const;
            bool remoteChanged() const;
            void writeCache();
            void applyCache();

        protected:
            void doPrint() const override;
            virtual void doSetRemoteEntry(const std::filesystem::path& path);

        private:
            bool syncInProgress = false;

            std::unique_ptr<RemoteEntry> remoteEntry;
            std::unique_ptr<Buffer> remoteBuffer;
            std::unique_ptr<Buffer> previousBuffer;

    };

    inline bool operator==(
        const Entry& lhs,
        const Entry& rhs) {
        
        return std::filesystem::equivalent(lhs.getAbsolutePath(), rhs.getAbsolutePath());
    }

    inline bool operator==(
        const Observer<Entry>& lhs,
        const Observer<Entry>& rhs) {
        
        return lhs.getId() == rhs.getId();
    }

}

#endif