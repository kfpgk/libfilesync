#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_TEST_HPP

namespace filesync::curl::storage::unit_test {

    class CharBufferTest {

        public:
            void test_clear();
            void test_get_size();
            void test_byte_array_and_string_write();
            void test_get_span();

            /**
             * @brief Test byte array write speed with
             * no preallocated memory.
             * 
             * Memory will have to be reallocated multiple
             * times, resulting in high durations.
             * 
             * Write 1MB in 1KB chunks.
             */
            void write_1MB_in_1KB_chunks_no_pre_alloc();
            /**
             * @brief Test byte array write speed with
             * preallocated memory.
             * 
             * No reallocations needed, so very efficient.
             * 
             * Write 1MB in 1KB chunks.
             */
            void write_1MB_in_1KB_chunks_with_pre_alloc();

            /**
             * @brief Test byte array read speed.
             * 
             * Read 1MB in 1KB chunks.
             */
            void read_1MB_in_1KB_chunks();

            /**
             * @brief Test byte array write speed with
             * preallocated memory.
             * 
             * No reallocations needed, so very efficient.
             * 
             * Write 1MB at once.
             */
            void write_1MB_at_once_with_pre_alloc();

            /**
             * @brief Test byte array read speed.
             * 
             * Read 1MB at once. 
             */
            void read_1MB_at_once();
            
    };

}

#endif