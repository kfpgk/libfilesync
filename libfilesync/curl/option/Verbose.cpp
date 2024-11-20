#include <libfilesync/curl/option/Verbose.hpp>
#include <libfilesync/curl/option/Option.hpp>

#include <curl/curl.h>

namespace filesync::curl::option {

    Verbose::Verbose(wrapper::Easy& curlEasy,
        bool value) :
        Option(curlEasy),
        value{value} {

    }

    Verbose::Verbose(wrapper::Easy& curlEasy,
        bool value,
        bool resetValue) :
        Option(curlEasy),
        value{value},
        ResettableOption(resetValue) {

    }

    Verbose::~Verbose() {
        reset();  
    }

    bool Verbose::getValue() {
        return value;
    }

    void Verbose::setTo(bool value) {
        if (value) {
            curlEasy.get().setOption(CURLOPT_VERBOSE, 1L);
        } else {
            curlEasy.get().setOption(CURLOPT_VERBOSE, 0L);
        }        
    }

}