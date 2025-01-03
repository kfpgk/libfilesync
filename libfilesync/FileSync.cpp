#include <libfilesync/FileSync.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/FileSyncLocks.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/core/sync_data/EntryFactory.hpp>
#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/core/BufferedSyncer.hpp>
#include <libfilesync/core/UnBufferedSyncer.hpp>
#include <libfilesync/core/OneWaySyncer.hpp>
#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/conflict/LocalFirstResolver.hpp>
#include <libfilesync/core/conflict/RemoteFirstResolver.hpp>
#include <libfilesync/core/conflict/InteractiveResolver.hpp>
#include <libfilesync/data/Exception.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

using namespace std::chrono_literals;
using namespace filesync::utility;

namespace filesync {

    /**
     * @brief Facade class for the file sync library.
     * 
     * Patterns:
     *  - Facade
     *  - Implementation of PIMPL
     */
    class FileSync::Impl {

        public:
            Impl() = default;
            ~Impl();
            Impl(const Impl&) = delete;
            Impl(Impl&&) = default;
            Impl operator=(Impl) = delete;

            void setProtocol(enum ProtocolType protocolType);
            void setServer(const std::string& address);
            void setRemoteRoot(const std::string& address);
            void setConflictResolveStrategy(enum ConflictResolveStrategy conflictResolveStrategy);
            void setSyncStrategy(enum SyncStrategy syncStrategy);
            void setSyncContent(const std::filesystem::path& path);
            void setSyncInvertal(std::chrono::milliseconds seconds);
            void setBufferType(enum BufferType bufferType);
            void startSyncing();
            void startSyncing(std::shared_ptr<FileSyncLocks> locks);
            void stopSyncing();

        private:
            std::string serverAddress = "";
            std::string remoteRoot = "";
            enum ProtocolType protocolType = ProtocolType::None;
            enum ConflictResolveStrategy conflictResolveStrategy = ConflictResolveStrategy::None;
            enum SyncStrategy syncStrategy = SyncStrategy::None;
            enum BufferType bufferType = BufferType::Default;
            std::chrono::milliseconds interval = 5s;
            std::shared_ptr<protocol::Client> protocolClient = nullptr;
            std::unique_ptr<core::sync_data::Entry> entry = nullptr;
            std::unique_ptr<core::conflict::Resolver> resolver = nullptr;
            std::unique_ptr<core::FileSyncer> fileSyncer = nullptr;

            std::thread syncThread;
            std::atomic<bool> syncing = false;
            std::atomic<bool> stopSyncingFlag = false;

            void createProtocol();
            void createConflictResolver(std::shared_ptr<FileSyncLocks> locks);
            void createFileSyncer();

            void validateProtocol() const;
            void validateEntry() const;
            void validateConflictResolver() const;

            /**
             * @brief Endless syncing loop for use 
             * in separate thread.
             */
            void endlessSync(const std::atomic<bool>& stop);

    };

    FileSync::Impl::~Impl() {
        if (syncing) {
            stopSyncing();
        }
    }

    void FileSync::Impl::setProtocol(enum ProtocolType protocolType) {
        this->protocolType = protocolType;     
    }

    void FileSync::Impl::setServer(const std::string& address) {
        this->serverAddress = address;     
    }

    void FileSync::Impl::setRemoteRoot(const std::string& remoteRoot) {
        this->remoteRoot = remoteRoot;
    }

    void FileSync::Impl::setConflictResolveStrategy(enum ConflictResolveStrategy conflictResolveStrategy) {
        this->conflictResolveStrategy = conflictResolveStrategy;
    }

    void FileSync::Impl::setSyncStrategy(enum SyncStrategy syncStrategy) {
        this->syncStrategy = syncStrategy;
    }

    void FileSync::Impl::setBufferType(enum BufferType bufferType) {
        this->bufferType = bufferType;
    }

    void FileSync::Impl::setSyncContent(const std::filesystem::path& path) {
        try {
            entry = filesync::core::sync_data::createSyncEntryRecursively(path);   
            Logger::getInstance().log(LogDomain::Info, "Added following files for syncing:");
            entry->print();
            Logger::getInstance().log(LogDomain::Info, "----------------------------------");  
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }     
    }

    void FileSync::Impl::setSyncInvertal(std::chrono::milliseconds interval) {
        this->interval = interval;
    }

    void FileSync::Impl::startSyncing() {
        if (syncing) {
            throw FileSyncException("Already syncing!",
                __FILE__, __LINE__);            
        }
        if (serverAddress.empty() || protocolType == ProtocolType::None) {
            throw FileSyncException("Protocol and Server must be set, before we start syncing!",
                __FILE__, __LINE__);
        }
        createProtocol();
        createConflictResolver(nullptr);    
        createFileSyncer();

        stopSyncingFlag = false;
        endlessSync(stopSyncingFlag);

    }

    void FileSync::Impl::startSyncing(std::shared_ptr<FileSyncLocks> locks) {
        if (syncing) {
            throw FileSyncException("Already syncing!",
                __FILE__, __LINE__);            
        }
        if (serverAddress.empty() || protocolType == ProtocolType::None) {
            throw FileSyncException("Protocol and Server must be set, before we start syncing!",
                __FILE__, __LINE__);
        }
        createProtocol();
        createConflictResolver(locks);    
        createFileSyncer();

        stopSyncingFlag = false;
        syncThread = std::thread(&FileSync::Impl::endlessSync, this, std::cref(stopSyncingFlag));

    }

    void FileSync::Impl::stopSyncing() {
        if (!syncing) {
            throw FileSyncException("Cannot stop, since sync has not been started!",
                __FILE__, __LINE__);            
        }
        stopSyncingFlag = true;
        syncThread.join();
    }

    void FileSync::Impl::createProtocol() {
        DEBUG_ENTER();

        switch (protocolType) {
            case ProtocolType::FTP:
                protocolClient = std::make_unique<protocol::FtpClient>(serverAddress, remoteRoot); 
                break;
            default:
                throw FileSyncException("Unsupported protocol type.",
                    __FILE__, __LINE__); 
        }

        DEBUG_EXIT();       
    }

    void FileSync::Impl::createConflictResolver(std::shared_ptr<FileSyncLocks> locks) {
        DEBUG_ENTER();

        switch (conflictResolveStrategy) {
            case ConflictResolveStrategy::LocalFirst:
                this->resolver = std::make_unique<core::conflict::LocalFirstResolver>(*protocolClient);
                break;
            case ConflictResolveStrategy::RemoteFirst:
                this->resolver = std::make_unique<core::conflict::RemoteFirstResolver>(*protocolClient);
                break;
            case ConflictResolveStrategy::Interactive:
                if (locks) {
                    this->resolver = std::make_unique<core::conflict::InteractiveResolver>(
                        *protocolClient, locks);
                } else {
                    this->resolver = std::make_unique<core::conflict::InteractiveResolver>(
                        *protocolClient);
                }
                
                break;
            default:
                throw FileSyncException("Unsupported conflict resolve strategy.",
                    __FILE__, __LINE__);                                                     
        }

        DEBUG_EXIT();
    }

    void FileSync::Impl::createFileSyncer() {
        DEBUG_ENTER();

        validateProtocol();
        validateEntry();

        switch (syncStrategy) {
            case SyncStrategy::UnBuffered:
                validateConflictResolver();
                this->fileSyncer = std::make_unique<core::UnbufferedSyncer>(
                    *entry, *protocolClient, *resolver);
                break;
            case SyncStrategy::Buffered:
                validateConflictResolver();
                this->fileSyncer = std::make_unique<core::BufferedSyncer>(
                    *entry, *protocolClient, *resolver);
                break;
            case SyncStrategy::OneWay:
                validateConflictResolver();
                this->fileSyncer = std::make_unique<core::OneWaySyncer>(
                    *entry, *protocolClient, *resolver);
                break;            
            default:
                throw FileSyncException("Unsupported FileSyncer type. Call setSyncStrategy()",
                    __FILE__, __LINE__);                
        }

        DEBUG_EXIT();
    }

    void FileSync::Impl::validateProtocol() const {
        if (!protocolClient) {
            throw FileSyncException("Need protocol client setup in order to create FileSyncer object. "\
                "Call setProtocol(...)",
            __FILE__, __LINE__);
        }
    }

    void FileSync::Impl::validateEntry() const {
        if (!entry) {
            throw FileSyncException("Need sync content setup in order to create FileSyncer object. "\
                "Call setSyncContent(...)",
            __FILE__, __LINE__);           
        }
    }

    void FileSync::Impl::validateConflictResolver() const {
        if (!resolver) {
            throw FileSyncException("Need conflict resolver setup in order to create FileSyncer object. "\
                "Call setConflictResolveStrategy(...)",
            __FILE__, __LINE__);           
        }
    }

    void FileSync::Impl::endlessSync(const std::atomic<bool>& stop) {
        syncing = true;
        constexpr std::chrono::duration pollInterval = 10ms;
        const int loopsPerSync = interval / pollInterval;
        int loopsUntilSync = 0;
        while (!stop) {               
            if (loopsUntilSync <= 0) {
                loopsUntilSync = loopsPerSync;
                entry->notify();
            }
            loopsUntilSync--;
            std::this_thread::sleep_for(pollInterval);
        }
        syncing = false;        
    }

    FileSync::FileSync() :
        pImpl{std::make_unique<FileSync::Impl>()} {

    }

    FileSync::~FileSync() = default;

    FileSync::FileSync(FileSync&&) = default;

    void FileSync::setProtocol(enum ProtocolType protocolType) {
        pImpl->setProtocol(protocolType);
    }

    void FileSync::setServer(const std::string& address) {
        pImpl->setServer(address);
    }

    void FileSync::setRemoteRoot(const std::string& address) {
        pImpl->setRemoteRoot(address);
    }

    void FileSync::setConflictResolveStrategy(
        enum ConflictResolveStrategy conflictResolveStrategy) {
        
        pImpl->setConflictResolveStrategy(conflictResolveStrategy);
    }

    void FileSync::setSyncStrategy(
        enum SyncStrategy syncStrategy) {
        
        pImpl->setSyncStrategy(syncStrategy);
    }
    
    void FileSync::setBufferType(enum BufferType bufferType) {
        pImpl->setBufferType(bufferType);
    }

    void FileSync::setSyncContent(const std::filesystem::path& path) {
        pImpl->setSyncContent(path);
    }

    void FileSync::setSyncInvertal(std::chrono::milliseconds seconds) {
        pImpl->setSyncInvertal(seconds);
    }

    void FileSync::startSyncing() {
        pImpl->startSyncing();
    }

    void FileSync::startSyncing(std::shared_ptr<FileSyncLocks> locks) {
        pImpl->startSyncing(std::move(locks));
    }

    void FileSync::stopSyncing() {
        pImpl->stopSyncing();
    }

}