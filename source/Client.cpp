#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <Client.hpp>
#include <Request.hpp>

using json = nlohmann::json;

Client::Client(std::string token)
{
    this->token = token;
}

void Client::initialize()
{
    printf("[initialize]\n");
    
    if (this->apiHost.empty())
    {
        this->apiHost = "https://api.music.yandex.net";
    }

    Request::initialize();
}

void Client::accountInfo()
{
    std::string url = this->apiHost + "/account/status";

    std::string result = Request::get(url);

    printf("[Client::accountInfo] %s\n", result.c_str());
}

void Client::search(std::string query, bool noCorrect, std::string type, int page)
{
    std::string url = this->apiHost + "/search";

    json urlParams = {
        { "text", query },
        { "nocorrect", noCorrect },
        { "type", type },
        { "page", page }
    };

    Request::addParams(url, urlParams);

    std::string result = Request::get(url);

    printf("[Client::search] %s\n", result.c_str());
}

Client::~Client()
{
    curl_global_cleanup();
}