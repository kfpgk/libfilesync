#ifndef LIBFILESYNC_TESTS_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP
#define LIBFILESYNC_TESTS_CORE_SYNC_DATA_BUFFER_FILE_BUFFER_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integration_test::core::sync_data::buffer::file_buffer {

    class FileBuffer : public IntegrationTest {

        public:
            explicit FileBuffer(const std::string& testName);

        private:
            const std::string inputFile1Name;
            const std::string inputFile1Content;
            const std::string inputFile2Name;
            const std::string inputFile2Content;
            const std::string flexibelFile1Name;
            const std::string flexibelFile1Content;
            bool result;
            bool resultOfIsEmpty;
            bool resultOfIsEqual;

            void setup() override;

            /**
             * @brief Test swap
             */
            void performSwap();
            void evaluateSwap();

            /**
             * @brief Test copy construction
             */
            void performCopyConstruction();
            void evaluateCopyConstruction();

            /**
             * @brief Test move construction
             */
            void performMoveConstruction();
            void evaluateMoveConstruction();

            /**
             * @brief Test copy assignment
             */
            void performCopyAssignment();
            void evaluateCopyAssignment();

            /**
             * @brief Test move assignment
             */
            void performMoveAssignment();
            void evaluateMoveAssignment();

            /**
             * @brief Test storing to and extracting from buffer.
             */
            void performStoreAndExtract();
            void evaluateStoreAndExtract();

            /**
             * @brief Test the check for equality 
             * with equal content.
             */
            void performCheckEqualityPositive();
            void evaluateCheckEqualityPositive();

            /**
             * @brief Test the check for equality 
             * with not equal content.
             */
            void performCheckEqualityNegative();
            void evaluateCheckEqualityNegative();

            /**
             * @brief Test the check for equality 
             * with empty buffer.
             */
            void performCheckEqualityEmptyBuffer();
            void evaluateCheckEqualityEmptyBuffer();

            /**
             * @brief Test the check for equality 
             * with empty compare data.
             */
            void performCheckEqualityEmptyCompData();
            void evaluateCheckEqualityEmptyCompData();

            /**
             * @brief Test the check for equality 
             * with empty buffer and empty compare
             * data.
             */
            void performCheckEqualityEmptyBufferAndEmptyData();
            void evaluateCheckEqualityEmptyBufferAndEmptyData();

    };

}

#endif