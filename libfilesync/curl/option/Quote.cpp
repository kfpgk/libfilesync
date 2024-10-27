#include <libfilesync/curl/option/Quote.hpp>
#include <libfilesync/curl/option/Option.hpp>

namespace filesync::curl::option {

    Quote::Quote(interface::Easy& curlEasy,
        const std::string& command) :
            Option(curlEasy),
            commands{new SList()} {
        
        addCommand(command);
    }

    Quote::Quote(interface::Easy& curlEasy,
        const std::string& command,
        SList* resetValue) :
            Option(curlEasy),
            ResettableOption(resetValue),
            commands{new SList()}  {

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

    SList* Quote::getValue() {
        return commands;
    }

    void Quote::setTo(SList* value) {
        if (value) {
            curlEasy.get().setOption(CURLOPT_QUOTE, value->getSListPointer()); 
        } else {
            curlEasy.get().setOption(CURLOPT_QUOTE, NULL); 
        }
    }

}