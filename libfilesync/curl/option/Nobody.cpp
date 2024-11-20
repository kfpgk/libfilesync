#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Option.hpp>

#include <curl/curl.h>

namespace filesync::curl::option {

    Nobody::Nobody(wrapper::Easy& interface,
        bool value) :
            Option(interface),
            value{value} {

    }

    Nobody::Nobody(wrapper::Easy& interface,
        bool value,
        bool resetValue) :
            Option(interface),
            value{value},
            ResettableOption(resetValue) {

    }

    Nobody::~Nobody() {
        reset();
    }

    bool Nobody::getValue() {
        return value;
    }

    void Nobody::setTo(bool value) {
        if (value) {
            interface.get().setOption(CURLOPT_NOBODY, 1L);
        } else {
            interface.get().setOption(CURLOPT_NOBODY, 0L);
        }        
    }

}