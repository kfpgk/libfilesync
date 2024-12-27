#ifndef LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HANDLE_TEST_HPP
#define LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HANDLE_TEST_HPP

namespace filesync::curl::storage::unit_test {

    /**
     * @brief MemoryStorageHandle unit test friend
     * class
     * 
     * This class is declared a friend class in
     * MemoryStorageHandle
     */
    class MemoryStorageHandleTest {

        public:

            void construct_from_friend();
            void construct_from_friend_with_make_unique();

    };

    /**
     * @brief MemoryStorageHandle unit test non
     * friend class
     * 
     * This class is not a friend class of
     * MemoryStorageHandle and thus is not
     * allowed to construct objects of it.
     * Implementation is uncommented because 
     * compile time error unit tests are not
     * implemented.
     */
    class MemoryStorageHandleClassNonFriend {

        public:
            void construct_from_non_friend();
            void construct_from_non_friend_with_make_unique();

    };

}

#endif