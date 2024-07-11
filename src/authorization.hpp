
#include <string>
#include <iostream>

class Authorization
{
private:
    std::string api_key;
    std::string api_secret;
    std::string api_id;
    std::string host;
public:
    std::string authorization;
public:
    Authorization(/* args */);
    ~Authorization();
    std::string get_current_timeRFC1123();
    std::string url_encode();
    std::string authorization_create();
    std::string hmac_sha256(const std::string& key, const std::string& message);
    std::string hmac_sha256(const std::string& key, const std::string& message, bool is_signature);
    std::string base64_encode(const unsigned char *data, size_t input_length);
    void set_api_key(std::string key);
    void set_api_secret(std::string secret);
    void set_api_id(std::string id);
    void set_host(std::string host);
private:
    void build_decoding_table();
    void signature_encode(const unsigned char *data, size_t input_length);
    std::string to_hex_string(const unsigned char* data, size_t length);
    std::string urlencode(const std::string& value);
    std::string date;
    std::string signature;
};

