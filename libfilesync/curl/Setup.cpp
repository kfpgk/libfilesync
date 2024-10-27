#include <libfilesync/curl/Setup.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace filesync::curl {

    Setup::Setup() {
        DEBUG("Running curl_global_init()");
        CURLcode ret = curl_global_init(CURL_GLOBAL_DEFAULT);       
        if (ret != 0) {
            throw Exception("curl_global_init() failed:", ret, __FILE__, __LINE__);
        }
    }

    Setup::~Setup() {
        DEBUG("Running curl_global_cleanup()");
        curl_global_cleanup();
    }

    /**
     * Instantiate the static singleton object to run
     * global curl setup (exactly once).
     */
    void Setup::init() {
        static Setup setup;
    }

}