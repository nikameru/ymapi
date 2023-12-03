#include <curl/curl.h>

#include <Client.hpp>
#include <Request.hpp>

Client::Client(std::string token)
{
    this->token = token;
}

void Client::initialize()
{
    printf("[initialize]\n");
    
    Request::initialize();
}

Client::~Client()
{
    curl_global_cleanup();
}