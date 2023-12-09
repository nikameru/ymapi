#include <iostream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <Client.hpp>
#include <Request.hpp>

using json = nlohmann::json;

static size_t writeResponseData(void *ptr, size_t size, size_t count, std::string *stream)
{
    printf("[writeResponse]\n");

    stream->append((char *)ptr, 0, size *count);

    return size *count;
}

void Request::initialize()
{
    printf("curl_global_init\n");

    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void Request::addParams(std::string &url, json params)
{
    CURL *curl;
    CURLUcode result;
    CURLU *urlHandle = curl_url();

    char *urlStr = nullptr;

    curl = curl_easy_init();
    result = curl_url_set(urlHandle, CURLUPART_URL, url.c_str(), 0);

    if (!result)
    {
        for (const auto &param : params.items())
        {
            const std::string key = param.key();
            const auto value = param.value();

            std::cout << "[Request::addParams] " << param.key() << "; " << param.value() << "\n";

            std::stringstream paramValue;
            paramValue << param.key() << "=";

            if (value.is_string())
            {
                std::string stringValue = value.get<std::string>();
                paramValue << curl_easy_escape(curl, stringValue.c_str(), stringValue.length());
            }
            else
            {
                paramValue << value;
            }

            printf("[Request::addParams] %s\n", paramValue.str().c_str());

            result = curl_url_set(urlHandle, CURLUPART_QUERY, paramValue.str().c_str(), CURLU_APPENDQUERY);
        }
    }

    result = curl_url_get(urlHandle, CURLUPART_URL, &urlStr, 0);

    if (urlStr != nullptr)
    {
        url = (std::string) urlStr;
        
        printf("[Request::addParams] new URL: %s\n", url.c_str());
    }
    else
    {
        printf("[Request::addParams] Unable to add parameters!\n");
    }

    curl_url_cleanup(urlHandle);
    curl_easy_cleanup(curl);
}

std::string Request::get(std::string url)
{
    std::string response;

    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;

    printf("[get] curl init\n");

    curl = curl_easy_init();

    if (curl)
    {
        headers = curl_slist_append(headers, "X-Yandex-Music-Client: YandexMusicAndroid/24023231");
        headers = curl_slist_append(headers, "accept: application/json");
        headers = curl_slist_append(headers, "Authorization: TOKEN");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Yandex-Music-API");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResponseData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        printf("[get] curl_easy_perform\n");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            printf("[get] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // In an actual app you should return an error on failure, following cleanup.
    }

    printf("[get] cleanup\n");

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}