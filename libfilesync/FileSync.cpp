#include <libfilesync/FileSync.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/core/LocalFirstFileSyncer.hpp>
#include <libfilesync/core/RemoteFirstFileSyncer.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/data/DataException.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

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
            void setSyncContent(const std::filesystem::path& path);
            void setSyncInvertal(std::chrono::milliseconds seconds);
            void startSyncing();
            void stopSyncing();

        private:
            std::string serverAddress = "";
            std::string remoteRoot = "";
            enum ProtocolType protocolType = ProtocolType::None;
            enum ConflictResolveStrategy conflictResolveStrategy = ConflictResolveStrategy::None;
            std::chrono::milliseconds interval = 5s;
            std::shared_ptr<ProtocolClient> protocolClient = nullptr;
            std::unique_ptr<Entry> entry = nullptr;
            std::unique_ptr<core::FileSyncer> fileSyncer = nullptr;

            std::thread syncThread;
            std::atomic<bool> syncing = false;
            std::atomic<bool> stopSyncThread = false;

            void createProtocol();
            void createFileSyncer();

            void validateProtocol() const;
            void validateEntry() const;

            /**
             * @brief Endless loop syncing for use 
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

    void FileSync::Impl::setSyncContent (const std::filesystem::path& path) {
        try {
            std::filesystem::path normalizedPath = path;
            normalizedPath.make_preferred();
            entry = filesync::data::createEntryRecursively(normalizedPath);
            if (path.string().back() == std::filesystem::path::preferred_separator) {
                entry->setRemote(normalizedPath.parent_path().filename());
            } else {
                entry->setRemote(normalizedPath.filename());
            }           
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
        createFileSyncer();

        stopSyncThread = false;
        syncThread = std::thread(&FileSync::Impl::endlessSync, this, std::cref(stopSyncThread));

    }

    void FileSync::Impl::stopSyncing() {
        if (!syncing) {
            throw FileSyncException("Cannot stop, since sync has not been started!",
                __FILE__, __LINE__);            
        }
        stopSyncThread = true;
        syncThread.join();
    }

    void FileSync::Impl::createProtocol() {
        DEBUG_ENTER();

        switch (protocolType) {
            case ProtocolType::FTP:
                protocolClient = std::make_unique<FtpClient>(serverAddress, remoteRoot); 
                break;
        }

        DEBUG_EXIT();       
    }

    void FileSync::Impl::createFileSyncer() {
        DEBUG_ENTER();

        validateProtocol();
        validateEntry();
        switch (conflictResolveStrategy) {
            case ConflictResolveStrategy::LocalFirst:
                this->fileSyncer = std::make_unique<core::LocalFirstFileSyncer>(*entry, *protocolClient);
                break;                
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
    
    void FileSync::setSyncContent(const std::filesystem::path& path) {
        pImpl->setSyncContent(path);
    }

    void FileSync::setSyncInvertal(std::chrono::milliseconds seconds) {
        pImpl->setSyncInvertal(seconds);
    }

    void FileSync::startSyncing() {
        pImpl->startSyncing();
    }

    void FileSync::stopSyncing() {
        pImpl->stopSyncing();
    }

}