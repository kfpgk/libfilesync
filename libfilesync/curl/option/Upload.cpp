#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/curl/option/Option.hpp>

namespace filesync::curl::option {

    Upload::Upload(interface::Easy& curlEasy,
        bool value) :
            Option(curlEasy),
            value{value} {

    }

    Upload::Upload(interface::Easy& curlEasy,
        bool value,
        bool resetValue) :
            Option(curlEasy),
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
            curlEasy.get().setOption(CURLOPT_UPLOAD, 1L);
        } else {
            curlEasy.get().setOption(CURLOPT_UPLOAD, 0L);
        }
        
    }

}