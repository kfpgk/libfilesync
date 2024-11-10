#ifndef LIBFILESYNC_FILE_SYNC_LOCKS_HPP
#define LIBFILESYNC_FILE_SYNC_LOCKS_HPP

#include <memory>
#include <mutex>
#include <experimental/propagate_const>

namespace filesync {

    enum class MutexType {stdCin};

    /**
     * @brief File Sync Locks for managing multi threaded
     * access.
     * 
     * Use this object to pass in mutexes that will be
     * used by the sync objects. Only needed for non blocking
     * sync.
     */
    class FileSyncLocks {

        public:
            FileSyncLocks();
            ~FileSyncLocks();
            FileSyncLocks(const FileSyncLocks&) = delete;
            FileSyncLocks(FileSyncLocks&&);
            FileSyncLocks operator=(FileSyncLocks) = delete;

            void addMutex(const MutexType& type, std::shared_ptr<std::mutex> mutex);
            std::mutex& getMutex(const MutexType& type);

        private:
            class Impl;
            std::experimental::propagate_const<
                std::unique_ptr<Impl>> pImpl;
    };

}

#endif