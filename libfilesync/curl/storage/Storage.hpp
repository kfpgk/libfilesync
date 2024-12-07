#ifndef LIBFILESYNC_CURL_STORAGE_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_STORAGE_HPP

#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/option/Factory.hpp>

namespace filesync::curl::storage {

    /**
     * @brief Storage interface class
     * 
     * Patterns:
     *  - Non virtual interface
     */
    class Storage {

        public:
            void setupRead(const option::Factory& optionFactory);
            void setupWrite(const option::Factory& optionFactory);
            void flush();

        private:
            virtual void doSetupRead(const option::Factory& optionFactory) = 0;
            virtual void doSetupWrite(const option::Factory& optionFactory) = 0;
            virtual void doFlush() = 0;
    };

}

#endif