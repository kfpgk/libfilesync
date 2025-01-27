#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_WRITE_BUFFER_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_WRITE_BUFFER_TEST_HPP

namespace filesync::curl::storage::char_buffer::unit_test {

    class ReadWriteBufferTest {

        public:
            void test_clear();
            void test_byte_array_and_string_write();

            void test_swap();
            void copy_construction();
            void move_construction();
            void copy_assignment();
            void move_assignment();

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