#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_HPP

#include <filesystem>

namespace filesync::core::sync_data {

    class Buffer {

        public:
            virtual ~Buffer() = default;

            void store();
            bool compareTo();

        private:
            virtual void doStore() = 0;
            virtual bool doCompareTo() = 0;

    };

}

#endif