#include <stdio.h>
#include <string>
#include <iostream>

#include <switch.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
//#include <pugixml/src/pugixml.hpp>

using json = nlohmann::json;

std::string SIGN_SALT = "XGRlBW9FXlekgbPrRHuSiA";

size_t writeResponse(void *ptr, size_t size, size_t count, std::string *stream)
{
    printf("writeResponse\n");
    stream->append((char *)ptr, 0, size *count);

    return size *count;
}

void getNetworkData(std::string url, std::string &responseContainer)
{
    CURL *curl;
    CURLcode res;

    printf("curl init\n");

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libnx curl example/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResponse);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseContainer);
        // Add any other options here.

        printf("curl_easy_perform\n");
        consoleUpdate(NULL);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
}

std::string buildDirectLink(std::string downloadInfoLink)
{
    printf("buildDirectLink");

    std::string downloadInfo;
    //pugi::xml_document downloadInfoDocument;

    getNetworkData(downloadInfoLink, downloadInfo);

    std::cout << downloadInfo;

    //pugi::xml_parse_result documentParsed = downloadInfoDocument.load_string(downloadInfo.c_str());

    //if (documentParsed)

    return "directLink";
}

std::string getTrackDownloadLink(int trackId)
{
    //CURL *curl;
    //CURLcode res;

    //curl_global_init(CURL_GLOBAL_DEFAULT);

    std::stringstream generalInfoLink;
    std::string generalInfo;
    json generalInfoJson;
    std::string directLink;
    
    generalInfoLink << "https://api.music.yandex.net/tracks/" << std::to_string(trackId) << "/download-info";

    std::cout << generalInfoLink.str().c_str() << "\n";

    getNetworkData(generalInfoLink.str(), generalInfo);

    printf("jsonParse\n");
    std::cout << generalInfo << "!!!\n";
    consoleUpdate(NULL);

    generalInfoJson = json::parse(generalInfo); 
    std::string downloadInfoLink = generalInfoJson["result"][0]["downloadInfoUrl"];

    printf("buildDirectLink");
    consoleUpdate(NULL);

    directLink = buildDirectLink(downloadInfoLink);

    std::cout << directLink << "\n";
    consoleUpdate(NULL);

    // In an actual app you should return an error on failure, following cleanup.

    printf("cleanup\n");
    //curl_easy_cleanup(curl);
    //curl_global_cleanup();

    return directLink;
}