#include <libfilesync/curl/option/Generic.hpp>

namespace filesync::curl::option {

    template <typename... Args>
    std::unique_ptr<Option> Factory::createGeneric(
        CURLoption curlOption, Args&&... args) const {

        return std::make_unique<option::Generic<Args...>>(
            curlEasy, curlOption, std::forward<Args>(args)...);
    }    

}