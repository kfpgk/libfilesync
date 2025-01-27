#ifndef LIBFILESYNC_CURL_OPTION_UNDOABLE_OPTION_IMPL_HPP
#define LIBFILESYNC_CURL_OPTION_UNDOABLE_OPTION_IMPL_HPP

#include <libfilesync/curl/option/UndoableOption.hpp>

#include <optional>

namespace filesync::curl::option {

    /**
     * @brief CURLOPT that provides an interface
     * for undoing  the option.
     */
    template<typename T>
    class UndoableOptionImpl : public UndoableOption {

        public:
            UndoableOptionImpl() = default;
            virtual ~UndoableOptionImpl() = default;

        private:
            std::optional<T> previous;
            
            void doSet() final;
            void doUndo() final;

            [[nodiscard]] virtual T getValue() = 0;
            virtual void setTo(T value) = 0;

    };

}

#include <libfilesync/curl/option/UndoableOptionImpl.tpp>

#endif