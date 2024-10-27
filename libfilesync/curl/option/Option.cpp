#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::option {

    Option::Option(interface::Easy& curlEasy) :
        curlEasy{curlEasy} {

    }

    void Option::setEasy(interface::Easy& curlEasy) {
        this->curlEasy = curlEasy;
    }

    void Option::set() {
        doSet();
    }

}