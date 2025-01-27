#include <libfilesync/curl/option/Verbose.hpp>
#include <libfilesync/curl/option/Option.hpp>

#include <curl/curl.h>

namespace filesync::curl::option {

    Verbose::Verbose(wrapper::Easy& interface,
        bool value) :
        Option(interface),
        value{value} {

    }

    Verbose::Verbose(wrapper::Easy& interface,
        bool value,
        bool resetValue) :
        Option(interface),
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
            interface.get().setOption(CURLOPT_VERBOSE, 1L);
        } else {
            interface.get().setOption(CURLOPT_VERBOSE, 0L);
        }        
    }

}