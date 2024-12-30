#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_TEST_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_TEST_HPP

namespace filesync::core::sync_data::buffer::unit_test {

    class ProtocolMemoryBufferTest {

        public:
            void is_equal_to_with_empty_buffer();
            void is_equal_to();
            void is_not_equal_to();
            void equal_start_but_input_is_larger();
            void equal_start_but_buffer_is_larger();

            void extract_content_to();

            void store_into_empty_buffer();
            void store_into_filled_buffer();

            void get_non_existing_handle();
            void get_valid_handle();
            void get_handle_twice();
           

    };

}

#endif