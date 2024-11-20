#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Option.hpp>

#include <curl/curl.h>

namespace filesync::curl::option {

    Nobody::Nobody(wrapper::Easy& curlEasy,
        bool value) :
            Option(curlEasy),
            value{value} {

    }

    Nobody::Nobody(wrapper::Easy& curlEasy,
        bool value,
        bool resetValue) :
            Option(curlEasy),
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
            curlEasy.get().setOption(CURLOPT_NOBODY, 1L);
        } else {
            curlEasy.get().setOption(CURLOPT_NOBODY, 0L);
        }        
    }

}