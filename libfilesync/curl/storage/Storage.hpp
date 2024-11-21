#ifndef LIBFILESYNC_CURL_STORAGE_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_STORAGE_HPP

#include <libfilesync/curl/option/Factory.hpp>

namespace filesync::curl {

    class ProtocolClient;

}

namespace filesync::curl::storage {

    class Storage {

        public:
            friend class filesync::curl::ProtocolClient;

        private:
            void setupRead(const option::Factory& optionFactory);
            void setupWrite(const option::Factory& optionFactory);
            void flush();

            virtual void doSetupRead(const option::Factory& optionFactory) = 0;
            virtual void doSetupWrite(const option::Factory& optionFactory) = 0;
            virtual void doFlush() = 0;

    };

}

#endif