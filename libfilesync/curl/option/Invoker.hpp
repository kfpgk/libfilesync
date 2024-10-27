#ifndef LIBFILESYNC_CURL_OPTION_INVOKER_HPP
#define LIBFILESYNC_CURL_OPTION_INVOKER_HPP

#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/option/UndoableOption.hpp>

#include <stack>
#include <memory>

namespace filesync::curl::option {

    /**
     * @brief Manages and keeps track of Option
     * objects
     * 
     * Patterns:
     *  - Invoker of the command pattern
     */
    class Invoker {

        public:
            void set(std::shared_ptr<Option> option);
            void undo();
            void redo();

        private:
            std::stack<std::shared_ptr<UndoableOption>> undoStack;
            std::stack<std::shared_ptr<UndoableOption>> redoStack;

    };

}

#endif