#ifndef LIBFILESYNC_DIRECTORY_TEST_HPP
#define LIBFILESYNC_DIRECTORY_TEST_HPP

namespace filesync {
namespace unittest {

    class DirectoryTest {

        public:
            void add_file();
            void add_directory();
            void remove_file();
            void remove_directory();

            void set_remotes_with_relative_local_paths();
            void set_remotes_with_deep_hierarchy();
            void set_remotes_with_gap_hierarchy();

    };

}
}

#endif