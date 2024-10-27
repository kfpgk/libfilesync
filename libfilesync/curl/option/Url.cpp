#include <libfilesync/curl/option/Url.hpp>

namespace filesync::curl::option {

    Url::Url(std::shared_ptr<interface::Easy> curlEasy,
        std::optional<bool> persistant,
        const std::string& host) :
            Option(curlEasy, persistant) {

        curlUrl.setHost(host);

    }

    void Url::setHost(const std::string& host) {
        curlUrl.setHost(host);
    }

    void Url::setScheme(const std::string& scheme) {
        curlUrl.setScheme(scheme);
    }

    void Url::setPath(const std::string& path) {
        curlUrl.setPath(path);      
    }

    void Url::doSet() {
        curlUrl.validate();
        previous = curlUrl;
        curlEasy->setOption(CURLOPT_URL, curlUrl.getUrl());
    }

    void Url::doReset() {

    }

    void Url::doUndo() {
        if (previous) {
            previous.value().curlUrl.validate();
            curlEasy->setOption(CURLOPT_URL, previous.value().curlUrl.getUrl());
        }
    }

}