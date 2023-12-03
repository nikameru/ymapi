#include <curl/curl.h>

#include <Request.hpp>

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