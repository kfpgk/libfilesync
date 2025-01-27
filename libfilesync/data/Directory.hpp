#ifndef LIBFILESYNC_DATA_DIRECTORY_HPP
#define LIBFILESYNC_DATA_DIRECTORY_HPP

#include <libfilesync/data/Entry.hpp>
#include <libfilesync/observer/Observer.hpp>

#include <deque>
#include <memory>
#include <filesystem>

namespace filesync::data {

    /**
     * Class representing a file system directory which
     * can be synced.
     * 
     * Patterns:
     *  - Composite object of the Composite pattern
     *  - Subject object of the Observer pattern
     */
    template<typename T>
    class DirectoryBase : virtual public EntryBase<T> {

        public:
            void addEntry(std::shared_ptr<T>);
            void removeEntry(const std::filesystem::path& path);

            void registerObserver(Observer<T>& observer) override;
            void unregisterObserver(Observer<T>& observer) override;
            void notify() override;

        protected:
            std::deque<std::shared_ptr<T>> components;

            explicit DirectoryBase(const std::filesystem::path& path);
        
        private:    
            virtual void doRemoveEntry(const std::filesystem::path& path);
            T* doGetEntry(const std::filesystem::path& path) override;
            void doPerformOnChange() override;
            [[nodiscard]] bool doValidate() const override;
            void doPrint() const override;          

    };

    class Directory : public Entry, public DirectoryBase<Entry> {

        public:
            explicit Directory(const std::filesystem::path& path);

    };

}

#include <libfilesync/data/Directory.tpp>

#endif