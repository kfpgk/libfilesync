#include <libfilesync/curl/storage/Storage.hpp>

namespace filesync::curl::storage {

    void Storage::setupRead(const option::Factory& optionFactory) {
        doSetupRead(optionFactory);
    }

    void Storage::setupWrite(const option::Factory& optionFactory) {
        doSetupWrite(optionFactory);
    }

    void Storage::flush() {
        doFlush();
    }

}