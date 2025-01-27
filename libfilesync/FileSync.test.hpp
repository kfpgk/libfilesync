#ifndef LIBFILESYNC_FILE_SYNC_TEST_HPP
#define LIBFILESYNC_FILE_SYNC_TEST_HPP

namespace filesync::unit_test {

    /**
     * @brief FileSync class unit test.
     * 
     * Includes only tests for things
     * that do not actually sync anything,
     * such as error handling.
     */
    class FileSyncTest {

        public:
            void missing_sync_content();
            void non_existent_sync_content();

    };

}

#endif