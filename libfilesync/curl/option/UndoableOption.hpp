#ifndef LIBFILESYNC_CURL_OPTION_UNDOABLE_OPTION_HPP
#define LIBFILESYNC_CURL_OPTION_UNDOABLE_OPTION_HPP

#include <libfilesync/curl/option/Option.hpp>

#include <optional>

namespace filesync::curl::option {

    /**
     * @brief Interface class for a CURLOPT which
     * can be undone.
     */
    class UndoableOption : virtual public Option {

        public:
            virtual ~UndoableOption() = default;
            void undo();

        private:
            virtual void doUndo() = 0;

    };

}

#endif