#ifndef LIBFILESYNC_ENTRY_HPP
#define LIBFILESYNC_ENTRY_HPP

#include <libfilesync/observer/Subject.hpp>
#include <libfilesync/observer/Observer.hpp>
#include <libfilesync/data/RemoteEntry.hpp>

#include <string>
#include <filesystem>
#include <optional>

namespace filesync 
{

    /** 
     * Class representing one or multiple file system 
     * entries (file or directory) which can be synced.
     * 
     * Patterns: 
     *  - Component object of the Composite pattern
     *  - Subject object of the Observer pattern
     *  - Non virtual interface
     */
    class Entry : public Subject<Entry> {

        public:
            virtual ~Entry() = default;
            Entry(const Entry&) = delete;
            Entry& operator=(const Entry&) = delete;
            
            [[nodiscard]] std::filesystem::path getPath() const;
            [[nodiscard]] std::filesystem::path getAbsolutePath() const;
            [[nodiscard]] Entry* getEntry(const std::filesystem::path& path); 
            [[nodiscard]] bool hasChanged() const;
            [[nodiscard]] bool hasRemote() const;
            [[nodiscard]] bool validate() const;
            [[nodiscard]] std::string getRemotePath() const;
            
            void resetChanged();
            void performOnChange();
            void setRemote(const std::string& path); 
            void print() const;

        protected:
            explicit Entry(const std::filesystem::path& path);

            [[nodiscard]]  virtual Entry* doGetEntry(const std::filesystem::path& path);
            virtual void doPerformOnChange();
            virtual void doSetRemote(const std::string& path);
            [[nodiscard]] virtual bool doValidate() const;
            virtual void doPrint() const;

        private:
            std::filesystem::path path;
            std::filesystem::file_time_type lastWriteTime;
            std::optional<RemoteEntry> remote;

            [[nodiscard]] std::string notFoundMessage() const;
            void onChange();

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