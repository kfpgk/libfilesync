#ifndef LIBFILESYNC_DIRECTORY_HPP
#define LIBFILESYNC_DIRECTORY_HPP

#include <libfilesync/data/Entry.hpp>

#include <deque>
#include <memory>
#include <filesystem>

namespace filesync {

    /**
     * Class representing a file system directory which
     * can be synced.
     * 
     * Patterns:
     *  - Composite object of the Composite pattern
     *  - Subject object of the Observer pattern
     */
    class Directory : public Entry {

        public:
            explicit Directory(const std::filesystem::path& path);
            void addEntry(std::shared_ptr<Entry>);
            void removeEntry(const std::filesystem::path& path);

            void registerObserver(Observer<Entry>& observer) override;
            void unregisterObserver(Observer<Entry>& observer) override;
            void notify() override;

        private:
            std::deque<std::shared_ptr<Entry>> components;

            Entry* doGetEntry(const std::filesystem::path& path) override;
            void doPerformOnChange() override;
            void doSetRemote(const std::string& path) override;
            [[nodiscard]] bool doValidate() const override;
            void doPrint() const override;          

    };

}

#endif