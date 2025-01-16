#ifndef LIBFILESYNC_DATA_ENTRY_HPP
#define LIBFILESYNC_DATA_ENTRY_HPP

#include <libfilesync/observer/Subject.hpp>
#include <libfilesync/observer/Observer.hpp>

#include <string>
#include <filesystem>


namespace filesync::data 
{

    /** 
     * @brief File System Entry CRTP base class template
     * 
     * Object representing one or multiple file system 
     * entries (file or directory).
     * 
     * Patterns: 
     *  - Component object of the Composite pattern
     *  - Subject object of the Observer pattern
     *  - Non virtual interface
     *  - CRTP
     */
    template<typename T>
    class EntryBase : public Subject<T> {

        public:
            virtual ~EntryBase() = default;
            EntryBase(const EntryBase&) = delete;
            EntryBase& operator=(const EntryBase&) = delete;
            
            [[nodiscard]] std::filesystem::path getPath() const;
            [[nodiscard]] std::filesystem::path getAbsolutePath() const;
            [[nodiscard]] T* getEntry(const std::filesystem::path& path); 
            [[nodiscard]] bool hasChanged() const;
            [[nodiscard]] bool validate() const;
            [[nodiscard]] bool isEqualTo(const std::filesystem::path& path) const;

            void resetChanged();
            void performOnChange();
            void print() const;

        protected:
            explicit EntryBase(const std::filesystem::path& path);

            [[nodiscard]]  virtual T* doGetEntry(const std::filesystem::path& path);
            virtual void doPerformOnChange();
            [[nodiscard]] virtual bool doValidate() const;
            virtual void doPrint() const;

        private:
            std::filesystem::path path;
            std::filesystem::file_time_type writeReferenceTime;

            [[nodiscard]] std::string notFoundMessage() const;
            void onChange();

    };

    /**
     * @brief File system entry concrete class
     */
    class Entry : virtual public EntryBase<Entry> {

        public:
            explicit Entry(const std::filesystem::path& path);

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

#include <libfilesync/data/Entry.tpp>

#endif