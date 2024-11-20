#ifndef LIBFILESYNC_CURL_WRAPPER_GLOBAL_SETUP_HPP
#define LIBFILESYNC_CURL_WRAPPER_GLOBAL_SETUP_HPP

#include <curl/curl.h>

namespace filesync::curl::wrapper {

    /**
     * @brief Global cURL setup class which makes sure
     * global init and global clean up is executed
     * only once.
     * 
     * Patterns:
     *  - Singleton
     */
    class GlobalSetup {

        public:            
            GlobalSetup(const GlobalSetup&) = delete;
            void operator=(const GlobalSetup&) = delete;

            void static init();

        private:
            GlobalSetup();
            ~GlobalSetup();
                        

    };

}

#endif