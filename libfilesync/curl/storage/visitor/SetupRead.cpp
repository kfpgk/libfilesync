#include <libfilesync/curl/storage/visitor/SetupRead.hpp>

namespace filesync::curl::storage::visitor {

    SetupRead::SetupRead(const option::Factory& optionFactory) :
        optionFactory{optionFactory} {

    }

    void SetupRead::operator()(FileStorage& storage) {
        storage.setupRead(optionFactory);
    }

    void SetupRead::operator()(MemoryStorage& storage) {
        storage.setupRead(optionFactory);
    }

}