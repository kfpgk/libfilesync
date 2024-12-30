#ifndef LIBFILESYNC_CORE_SYNC_DATA_ENTRY_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_ENTRY_HPP

#include <libfilesync/data/Entry.hpp>
#include <libfilesync/observer/Observer.hpp>
#include <libfilesync/core/sync_data/RemoteEntry.hpp>
#include <libfilesync/core/sync_data/buffer/Buffer.hpp>
#include <libfilesync/protocol/HandleOrFilePath.hpp>

#include <filesystem>
#include <memory>
#include <variant>

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
            void resetSyncInProgress();
            [[nodiscard]] bool getSyncInProgress() const;
            /**
             * @brief Check if local file content is different
             * compared to content in previous buffer.
             * 
             * @returns true if contents are different. false
             * otherwise.
             */
            [[nodiscard]] bool localDifferentThanPrev();
            /**
             * @brief Get file pathhandle or to the data in 
             * remote buffer.
             * 
             * This can be used to write to a buffer.
             * 
             * @returns a value of variable type depending on the 
             * underlying buffer that is used.
             */
            protocol::HandleOrFilePath getRemoteBufferHandle();
            /**
             * @brief Check if content in remote buffer is different
             * compared to content in previous buffer.
             * 
             * @returns true if contents are different. false
             * otherwise.
             */
            [[nodiscard]] bool remoteDifferentThanPrev();
            /**
             * @brief Saves the current state of the local file
             * into a buffer for later comparison.
             */
            void setPrevious();
            /**
             * @brief Overwrite the local entry with the contents
             * of the remote buffer.
             */
            void writeRemoteBufferToLocal();

        protected:
            void doPrint() const override;
            virtual void doSetRemoteEntry(const std::filesystem::path& path);
            virtual void doSetSyncInProgress();
            virtual void doResetSyncInProgress();
            [[nodiscard]] virtual bool doGetSyncInProgress() const;
            [[nodiscard]] virtual bool doLocalDifferentThanPrev();
            [[nodiscard]] virtual bool doRemoteDifferentThanPrev();
            virtual void doSetPrevious();
            virtual void doWriteRemoteBufferToLocal();

        private:
            bool syncInProgress = false;

            std::unique_ptr<RemoteEntry> remoteEntry;
            buffer::Buffer bufferForRemote;
            buffer::Buffer bufferForPrevious;

    };

    inline bool operator==(
        const Entry& lhs,
        const Entry& rhs) {

        namespace fs = std::filesystem;
        
        return(
            fs::weakly_canonical(lhs.getAbsolutePath()) ==
            fs::weakly_canonical(rhs.getAbsolutePath())
        );
    }

    inline bool operator==(
        const Observer<Entry>& lhs,
        const Observer<Entry>& rhs) {
        
        return lhs.getId() == rhs.getId();
    }

}

#endif