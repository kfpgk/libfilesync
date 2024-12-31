#ifndef LIBFILESYNC_CORE_SYNC_DATA_ENTRY_TEST_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_ENTRY_TEST_HPP

namespace filesync::core::sync_data::unit_test {

    class EntryTest {

        public:
            void set_and_get_remote_entry();
            void get_default_remote_entry();
            void get_default_sync_in_progress();

            void set_sync_in_progress();
            void reset_sync_in_progress();
            
            void equality_operator_with_equals();
            void equality_operator_with_not_equals();

            void construct_with_file_buffers();
            void construct_with_memory_buffers();
            void construct_remote_with_char_array_buffer();
            void construct_previous_with_protocol_buffer();

    };

}

#endif