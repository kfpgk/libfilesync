#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::option {

    Option::Option(wrapper::Easy& interface) :
        interface{interface} {

    }

    void Option::setEasy(wrapper::Easy& interface) {
        this->interface = interface;
    }

    void Option::set() {
        doSet();
    }

}