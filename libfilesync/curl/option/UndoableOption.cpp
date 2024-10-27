#include <libfilesync/curl/option/UndoableOption.hpp>

namespace filesync::curl::option {

    void UndoableOption::undo() {
        doUndo();
    }

}