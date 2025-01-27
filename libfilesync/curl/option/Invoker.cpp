#include <libfilesync/curl/option/Invoker.hpp>

namespace filesync::curl::option {

    void Invoker::set(std::shared_ptr<Option> option) {
        option->set();
        std::shared_ptr<UndoableOption> undoableOption = 
            std::dynamic_pointer_cast<UndoableOption>(option);
        if (undoableOption) {
            undoStack.push(undoableOption);
        }
    }

    void Invoker::undo() {
        if (!undoStack.empty()) {
            undoStack.top()->undo();
            redoStack.push(undoStack.top());
            undoStack.pop();
        }
    }

    void Invoker::redo() {
        if (!redoStack.empty()) {
            redoStack.top()->set();
            undoStack.push(redoStack.top());
            redoStack.pop();
        }        
    }

}