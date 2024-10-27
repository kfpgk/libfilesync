#include <libfilesync/curl/Base.hpp>
#include <libfilesync/curl/Setup.hpp>

namespace filesync::curl {

    Base::Base() {
        Setup::init();
    }

}