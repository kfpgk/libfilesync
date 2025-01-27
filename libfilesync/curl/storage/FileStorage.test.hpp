#ifndef LIBFILESYNC_CURL_STORAGE_FILE_STORAGE_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_FILE_STORAGE_TEST_HPP

namespace filesync::curl::storage::unit_test {

    class FileStorageTest {

        public:
            void set_input_filesize_to_0();
            void set_input_filesize_to_1KB();         
            void set_input_filesize_to_2047MB();
            void set_input_filesize_to_2GB();
            void set_input_filesize_to_99TB();
            void set_input_filesize_to_n1KB();

    };

}

#endif