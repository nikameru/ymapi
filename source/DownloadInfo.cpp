#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include <switch.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <md5/md5.h>

#include <Request.hpp>

using json = nlohmann::json;

const std::string SIGN_SALT = "XGRlBW9FXlekgbPrRHuSiA";

std::string hexDigest(std::string s)
{
    printf("hexDigest\n");
    std::cout << s << "\n\n";

    std::ostringstream hex;

    for (int i = 0; i < (int) s.length(); i++)
        hex << std::hex << std::uppercase << (int) s[i];

    return hex.str();
}

std::string getXmlValue(std::string xml, std::string tag)
{
    size_t beginIndex = xml.find(tag) + tag.length();
    size_t endIndex = xml.find(tag.insert(1, 1, '/'));

    return xml.substr(beginIndex, endIndex - beginIndex);
}

std::string buildDirectLink(std::string downloadInfoLink)
{
    printf("buildDirectLink");

    std::string downloadInfo = Request::get(downloadInfoLink);

    std::cout << downloadInfo << "\n\n";

    std::string host = getXmlValue(downloadInfo, "<host>");
    std::string path = getXmlValue(downloadInfo, "<path>");
    std::string ts = getXmlValue(downloadInfo, "<ts>");
    std::string s = getXmlValue(downloadInfo, "<s>");

    std::string sign = hexDigest(md5(SIGN_SALT + path.substr(1) + s));

    std::cout << host << "\n"
              << path << "\n"
              << ts << "\n"
              << s << "\n";

    std::stringstream directLink;
    directLink << "https://" << host << "/get-mp3/" << sign << "/" << ts << path;

    return directLink.str();
}

std::string getTrackDownloadLink(int trackId)
{
    std::stringstream generalInfoLink;
    std::string generalInfo;

    generalInfoLink << "https://api.music.yandex.net/tracks/" << std::to_string(trackId) << "/download-info";

    std::cout << generalInfoLink.str().c_str() << "\n";

    generalInfo = Request::get(generalInfoLink.str());

    printf("jsonParse\n");
    std::cout << generalInfo << "!!!\n";

    json generalInfoJson = json::parse(generalInfo);
    std::string downloadInfoLink = generalInfoJson["result"][0]["downloadInfoUrl"];

    std::string directLink = buildDirectLink(downloadInfoLink);

    std::cout << directLink << "\n";

    return directLink;
}