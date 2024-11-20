#ifndef LIBFILESYNC_CURL_UNIT_TEST_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_CURL_UNIT_TEST_PROTOCOL_CLIENT_HPP

#include <libfilesync/curl/ProtocolClient.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>

#include <memory>

namespace filesync::curl::unittest {

    class ProtocolClientConcrete : public curl::ProtocolClient {

        public:
            ProtocolClientConcrete(const std::string& serverAddress,
                std::unique_ptr<wrapper::Easy> curlEasy) :
                    ProtocolClient(std::move(curlEasy)) {

                activeUrl.setHost(serverAddress);
                activeUrl.setScheme("http");
                activeUrl.validate();        
                std::unique_ptr<option::Option> option =
                    optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl());
                option->set();
            }

        private:
            void doDeleteRemoteFile() override {} ;
            void doCreateRemoteDir() override {} ;
            void doDeleteRemoteDir() override {} ;
            void doSetCreateMissingDirs([[maybe_unused]] bool value) override {} ;
            
    };

    class ProtocolClient {

        public:
            void set_bad_host_name();

            void set_input_filesize_to_0();
            void set_input_filesize_to_1KB();         
            void set_input_filesize_to_2047MB();
            void set_input_filesize_to_2GB();
            void set_input_filesize_to_99TB();
            void set_input_filesize_to_n1KB();

            void set_valid_remote_file();
            void set_remote_file_with_special_chars();

            void set_non_existing_local_file_for_upload();

            void download_uninitialized_easy_stubbed();
            void download_with_uninitialized_local_path_easy_stubbed();

            void upload_uninitialized_easy_stubbed();
            void upload_with_uninitialized_local_path_easy_stubbed();

    };

}

#endif