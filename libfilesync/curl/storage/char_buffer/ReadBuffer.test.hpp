#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_BUFFER_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_BUFFER_TEST_HPP

namespace filesync::curl::storage::char_buffer::unit_test {

    class ReadBufferTest {

        public:
            void test_get_size();
            void test_get_span();
            void test_get_string();

            void test_swap();
            void copy_construction();
            void move_construction();
            void copy_assignment();
            void move_assignment();

            /**
             * @brief Test byte array read speed.
             * 
             * Read 1MB in 1KB chunks.
             */
            void read_1MB_in_1KB_chunks();

            /**
             * @brief Test byte array read speed.
             * 
             * Read 1MB at once. 
             */
            void read_1MB_at_once();
            
    };

}

#endif