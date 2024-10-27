#ifndef LIBFILESYNC_CURL_OPTION_URL_HPP
#define LIBFILESYNC_CURL_OPTION_URL_HPP

#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/interface/Easy.hpp>
#include <libfilesync/curl/Url.hpp>

#include <string>
#include <memory>
#include <optional>

namespace filesync::curl::option {

    class Url : public Option {

        public:
            Url(std::shared_ptr<interface::Easy> curlEasy,
                std::optional<bool> persistent,
                const std::string& host);

            void setHost(const std::string& host);
            void setScheme(const std::string& scheme);
            void setPath(const std::string& path);

        private:
            curl::Url curlUrl;
            std::optional<option::Url> previous;

            void doSet() override;
            void doReset() override;
            void doUndo() override;
    };

}

#endif