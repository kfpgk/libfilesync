#include <libfilesync/curl/option/Collection.hpp>

namespace filesync::curl::option {

    Collection::Collection(wrapper::Easy& interface) :
            Option(interface) {

    }

    void Collection::add(
        std::shared_ptr<Option> option) {

        options.push_back(std::move(option));
    }

    void Collection::clear() {
        options.clear();
    }

    void Collection::doSet() {
        for (auto it = options.begin(); it != options.end(); it++) {
            it->get()->set();
        }
    }

}