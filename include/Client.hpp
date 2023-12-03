#include <string>

class Client
{
private:
    std::string token;  

public:
    Client(std::string token);
    
    void initialize();

    ~Client();
};