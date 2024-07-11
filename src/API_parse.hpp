#ifndef API_parse_HPP
#define API_parse_HPP

#include <iostream>
#include <string>

class API_parse
{
public:
    API_parse(/* args */);
    ~API_parse();
    int parse_json(const std::string& file_name);
    std::string api_id;
    std::string api_secret;
    std::string api_key;
private:
    
};



#endif // !API_parse_HPP

