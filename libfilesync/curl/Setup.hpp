#ifndef LIBFILESYNC_CURL_SETUP_HPP
#define LIBFILESYNC_CURL_SETUP_HPP

#include <curl/curl.h>

namespace filesync::curl {

    /**
     * @brief Global cURL setup class which makes sure
     * global init and global clean up is executed
     * only once.
     * 
     * Patterns:
     *  - Singleton
     */
    class Setup {

        public:            
            Setup(const Setup&) = delete;
            void operator=(const Setup&) = delete;

            void static init();

        private:
            Setup();
            ~Setup();
                        

    };

}

#endif