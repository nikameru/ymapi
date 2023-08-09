#include <stdio.h>
#include <string>

#include <switch.h>
#include <curl/curl.h>

std::string SIGN_SALT = "XGRlBW9FXlekgbPrRHuSiA";

void yandex_request(void)
{
    CURL *curl;
    CURLcode res;

    printf("Initiating curl...\n\n");
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.music.yandex.net/tracks/253451/download-info");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Yandex-Music-API");

        printf("Requesting...\n\n");
        consoleUpdate(NULL);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed: %s\n\n", curl_easy_strerror(res));
        }
        else
        {
            printf(res + "\n\n");
        }

        printf("Freeing memory...\n\n");
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}