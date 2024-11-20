#include <libfilesync/curl/wrapper/Base.hpp>
#include <libfilesync/curl/wrapper/GlobalSetup.hpp>

namespace filesync::curl::wrapper {

    Base::Base() {
        GlobalSetup::init();
    }

}