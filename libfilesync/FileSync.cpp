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

using namespace filesync::utility;

namespace filesync {

    FileSync::~FileSync() {
        if (syncing) {
            stopSyncing();
        }
    }

    void FileSync::setProtocol(enum ProtocolType protocolType) {
        this->protocolType = protocolType;     
    }

    void FileSync::setServer(const std::string& address) {
        this->serverAddress = address;     
    }

    void FileSync::setRemoteRoot(const std::string& remoteRoot) {
        this->remoteRoot = remoteRoot;
    }

    void FileSync::setConflictResolveStrategy(enum ConflictResolveStrategy conflictResolveStrategy) {
        this->conflictResolveStrategy = conflictResolveStrategy;
    }

    void FileSync::setSyncContent (const std::filesystem::path& path) {
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

    void FileSync::setSyncInvertal(std::chrono::milliseconds interval) {
        this->interval = interval;
    }

    void FileSync::startSyncing() {
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
        syncThread = std::thread(&FileSync::endlessSync, this, std::cref(stopSyncThread));

    }

    void FileSync::stopSyncing() {
        if (!syncing) {
            throw FileSyncException("Cannot stop, since sync has not been started!",
                __FILE__, __LINE__);            
        }
        stopSyncThread = true;
        syncThread.join();
    }

    void FileSync::createProtocol() {
        DEBUG_ENTER();

        switch (protocolType) {
            case ProtocolType::FTP:
                protocolClient = std::make_unique<FtpClient>(serverAddress, remoteRoot); 
                break;
        }

        DEBUG_EXIT();       
    }

    void FileSync::createFileSyncer() {
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

    void FileSync::validateProtocol() const {
        if (!protocolClient) {
            throw FileSyncException("Need protocol client setup in order to create FileSyncer object. "\
                "Call setProtocol(...)",
            __FILE__, __LINE__);
        }
    }

    void FileSync::validateEntry() const {
        if (!entry) {
            throw FileSyncException("Need sync content setup in order to create FileSyncer object. "\
                "Call setSyncContent(...)",
            __FILE__, __LINE__);           
        }
    }

    void FileSync::endlessSync(const std::atomic<bool>& stop) {
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

}