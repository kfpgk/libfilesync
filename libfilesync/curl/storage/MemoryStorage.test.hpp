#ifndef LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_TEST_HPP

namespace filesync::curl::storage::unit_test {

    class MemoryStorageTest {

        public:
            void test_swap_one_empty();
            void test_swap_read_write_vs_read_only();
            void copy_construction();
            void move_construction();
            void copy_assignment();
            void move_assignment();

    };
}

#endif