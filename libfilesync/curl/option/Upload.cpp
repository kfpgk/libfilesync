#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/curl/option/Option.hpp>

namespace filesync::curl::option {

    Upload::Upload(wrapper::Easy& interface,
        bool value) :
            Option(interface),
            value{value} {

    }

    Upload::Upload(wrapper::Easy& interface,
        bool value,
        bool resetValue) :
            Option(interface),
            value{value},
            ResettableOption(resetValue) {

    }

    Upload::~Upload() {
        reset();   
    }

    bool Upload::getValue() {
        return value;
    }

    void Upload::setTo(bool value) {
        if (value) {
            interface.get().setOption(CURLOPT_UPLOAD, 1L);
        } else {
            interface.get().setOption(CURLOPT_UPLOAD, 0L);
        }
        
    }

}