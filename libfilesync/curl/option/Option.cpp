#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::option {

    Option::Option(wrapper::Easy& curlEasy) :
        curlEasy{curlEasy} {

    }

    void Option::setEasy(wrapper::Easy& curlEasy) {
        this->curlEasy = curlEasy;
    }

    void Option::set() {
        doSet();
    }

}