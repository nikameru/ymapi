#pragma once

#include <string>

class Client
{
protected:
    std::string token = "";  
    std::string apiHost = "";

public:
    Client(std::string token);
    
    void initialize();
    void accountInfo();
    //void getLikes(int uid);
    void search(std::string query, bool noCorrect, std::string type, int page);

    ~Client();
};