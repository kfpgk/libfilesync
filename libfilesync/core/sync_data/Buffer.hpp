#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_HPP

#include <istream>

namespace filesync::core::sync_data {

    /**
     * @brief Abstract data buffer interface
     * 
     * Patterns:
     *  - Non virtual interface
     */
    class Buffer {

        public:
            virtual ~Buffer() = default;

            void store(std::istream& in);
            /**
             * @brief Writes the buffers content
             * to the 'out' stream parameter.
             * 
             * Returns false if no content is available
             * to be written. Returns true if content
             * has been written to 'out'.
             */
            bool writeContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in);

        private:
            virtual void doStore(std::istream& in) = 0;
            virtual bool doWriteContentTo(std::ostream& out) = 0;
            virtual bool doIsEqualTo(std::istream& in) = 0;
            
    };

}

#endif