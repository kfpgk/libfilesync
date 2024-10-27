#ifndef LIBFILESYNC_FILE_SYNC_EXCEPTION_HPP
#define LIBFILESYNC_FILE_SYNC_EXCEPTION_HPP

#include <string>
#include <vector>
#include <stdexcept>

namespace filesync {

    class FileSyncException : public std::runtime_error {

        public:
            explicit FileSyncException(const std::string& errorMessage);
            FileSyncException(const std::string& errorMessage,
                const std::string& context);
            FileSyncException(const std::string& errorMessage,
                const char *file,
                int line);
            virtual ~FileSyncException() = default;

            const char* what() const noexcept override;
            /**
             * @brief Constructs the complete error message
             * from contexts and error message and returns
             * it as string.
             */
            std::string getCompleteErrorMessage() const;
            std::string getErrorMessage() const;
            void setErrorMessage(const std::string& message);
            void appendToErrorMessage(const std::string& message);
            void addContext(const std::string& context);
            void addContext(const char *file, int line);          

        private:
            /**
             * @brief The complete error message including context
             * and error message.
             * 
             * This is printed by what(). It is constructed during
             * what() which has to be const to conform to standard
             * prototype. This is why it is declared "mutable".
             * 
             * This member is also the buffer for the const char*
             * that is being returned by what(). Must be valid
             * after returning from what().
             */
            mutable std::string completeErrorMessageBuffer;
            std::string errorMessage;           
            std::vector<std::string> contexts;

    };

}

#endif