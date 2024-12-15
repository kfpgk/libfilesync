#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_TEST_HPP

namespace filesync::curl::storage::unit_test {

    class CharBufferTest {

        public:
            void test_clear();
            void test_get_size();
            void test_byte_array_and_string_write();
            void test_get_span();
            
    };

}

#endif