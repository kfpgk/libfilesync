#include <libfilesync/FileSyncLocks.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <unordered_map>

namespace filesync {

    class FileSyncLocks::Impl {

        public:
            void addMutex(const MutexType& type, std::shared_ptr<std::mutex> mutex);
            std::mutex& getMutex(const MutexType& type);

        private:
            std::unordered_map<const MutexType, std::shared_ptr<std::mutex>> locks {
                {MutexType::stdCin, nullptr}
            };

    };

    void FileSyncLocks::Impl::addMutex(const MutexType& type, std::shared_ptr<std::mutex> mutex) {
        if (locks[type]) {
            throw FileSyncException("Mutex already created. Cannot overwrite. Call this before "\
                "before starting syncing.",
                __FILE__, __LINE__);            
        }
        locks[type] = mutex;
    }
    
    std::mutex& FileSyncLocks::Impl::getMutex(const MutexType& type) {
        if (locks.count(type) == 0) {
            throw FileSyncException("Mutex map does not support this mutex type.",
                __FILE__, __LINE__);
        }
        if (!locks[type]) {
            locks[type] = std::make_shared<std::mutex>();
        }
        return *locks[type];
    }    

    FileSyncLocks::FileSyncLocks() :
        pImpl(std::make_unique<FileSyncLocks::Impl>()) {

    }

    FileSyncLocks::~FileSyncLocks() = default;

    FileSyncLocks::FileSyncLocks(FileSyncLocks&&) = default;

    void FileSyncLocks::addMutex(const MutexType& type, std::shared_ptr<std::mutex> mutex) {
        pImpl->addMutex(type, mutex);
    }
    
    std::mutex& FileSyncLocks::getMutex(const MutexType& type) {
        return pImpl->getMutex(type);
    }

}