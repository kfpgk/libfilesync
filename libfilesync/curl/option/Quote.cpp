#include <libfilesync/curl/option/Quote.hpp>
#include <libfilesync/curl/option/Option.hpp>

namespace filesync::curl::option {

    Quote::Quote(wrapper::Easy& interface,
        const std::string& command) :
            Option(interface),
            commands{new wrapper::SList()} {
        
        addCommand(command);
    }

    Quote::Quote(wrapper::Easy& interface,
        const std::string& command,
        wrapper::SList* resetValue) :
            Option(interface),
            ResettableOption(resetValue),
            commands{new wrapper::SList()}  {

        addCommand(command);       
    }

    Quote::~Quote() {
        reset();
        delete commands;   
    }

    void Quote::addCommand(const std::string& command) {
        if (!command.empty()) {
            commands->append(command);
        }
    }    

    wrapper::SList* Quote::getValue() {
        return commands;
    }

    void Quote::setTo(wrapper::SList* value) {
        if (value) {
            interface.get().setOption(CURLOPT_QUOTE, value->getSListPointer()); 
        } else {
            interface.get().setOption(CURLOPT_QUOTE, NULL); 
        }
    }

}