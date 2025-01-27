#ifndef LIBFILESYNC_TESTS_CURL_STORAGE_FILE_STORAGE_FILE_STORAGE_HPP
#define LIBFILESYNC_TESTS_CURL_STORAGE_FILE_STORAGE_FILE_STORAGE_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integration_test::curl::storage::file_storage {

    class FileStorage : public IntegrationTest {

        public:
            FileStorage(const std::string& name);

        private:
            const std::string inputFile1Name;
            const std::string inputFile1Content;
            const std::string inputFile2Name;
            const std::string inputFile2Content;
            bool result;
            bool resultOfPath1;
            bool resultOfPath2;
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
    };

}

#endif