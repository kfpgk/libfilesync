#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::option {

    template<typename T>
    void UndoableOptionImpl<T>::doUndo() {
        if (previous) {
            setTo(previous.value());
        }  
    }

    template<typename T>
    void UndoableOptionImpl<T>::doSet() {
        setTo(getValue());
        previous = getValue();
    }

}