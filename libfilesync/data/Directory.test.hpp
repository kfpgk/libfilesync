#ifndef LIBFILESYNC_DATA_DIRECTORY_TEST_HPP
#define LIBFILESYNC_DATA_DIRECTORY_TEST_HPP

namespace filesync::data::unittest {

    class DirectoryTest {

        public:
            void add_file();
            void add_directory();
            void remove_file();
            void remove_directory();

    };

}

#endif