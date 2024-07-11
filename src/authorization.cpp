#include "authorization.hpp"
#include <openssl/bio.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/' };
static char *decoding_table = NULL;
static int mod_table[] = { 0, 2, 1 };

Authorization::Authorization(/* args */)
{
}

Authorization::~Authorization()
{
}

void Authorization::build_decoding_table()
{
	decoding_table = (char*)malloc(256);
 
	for (int i = 0; i < 64; i++)
		decoding_table[(unsigned char)encoding_table[i]] = i;
}

std::string Authorization::base64_encode(const unsigned char *data, size_t input_length) {
    size_t output_length = 4 * ((input_length + 2) / 3);
    std::string encoded_data(output_length, '\0');
    
    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';

    return encoded_data;
}

std::string Authorization::urlencode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << std::uppercase << static_cast<int>(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}


std::string Authorization::url_encode() {
    authorization_create();
    // std::unordered_map<std::string, std::string> params = {
    //     {"authorization", authorization},
    //     {"date", date},
    //     {"host", host}
    // };

    // std::ostringstream query;
    // for (auto it = params.begin(); it != params.end(); ++it) {
    //     if (it != params.begin()) {
    //         query << "&";
    //     }
    //     query << urlencode(it->first) << "=" << urlencode(it->second);
    // }

    std::ostringstream oss;
    oss << "https://spark-api.cn-huabei-1.xf-yun.com/v2.1/image?"
        << "authorization=" << urlencode(authorization) << "&"
        << "date=" << urlencode(date) << "&"
        << "host=" << urlencode(host);

    std::string final_url = oss.str();

    std::string url = oss.str();
    return url;
}
std::string Authorization::get_current_timeRFC1123()
{
    std::time_t now = std::time(nullptr);
    std::tm* gmtm = std::gmtime(&now);

    std::stringstream ss;
    ss << std::put_time(gmtm, "%a, %d %b %Y %H:%M:%S GMT");
    date = ss.str();
    return ss.str();
}

std::string Authorization::authorization_create()
{
    std::string tmp = "host: " + host + "\n";
    tmp += "date: " + get_current_timeRFC1123() + "\n";
    tmp += "GET /v2.1/image HTTP/1.1";
    std::cout << "tmp: " << tmp << std::endl;
    std::string tmp_sha = hmac_sha256(api_secret, tmp, true);
    std::cout << "tmp_sha: " << tmp_sha << std::endl;
    std::string authorization_origin = "api_key=\"" + api_key + "\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"" + signature + "\"";
    std::cout << "authorization_origin: " << authorization_origin << std::endl;
    std::string authorization = base64_encode((const unsigned char *)authorization_origin.data(), authorization_origin.length());
    std::cout << "authorization: " << authorization << std::endl;
    this->authorization = authorization;
    return authorization;
}

// 将二进制数据转换为十六进制字符串
std::string Authorization::to_hex_string(const unsigned char* data, size_t length) {
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return oss.str();
}

// 计算HMAC-SHA256哈希值
std::string Authorization::hmac_sha256(const std::string& key, const std::string& message) 
{
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int len = 0;

    // 计算HMAC-SHA256哈希值
    // HMAC(EVP_sha256(), key.c_str(), key.length(),
    //      reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), result, &len);
    HMAC_CTX* ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha256(), nullptr);
    HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
    HMAC_Final(ctx, result, &len);
    HMAC_CTX_free(ctx);
    // 将二进制哈希值转换为std::string
    return to_hex_string(result, len);
}

std::string Authorization::hmac_sha256(const std::string& key, const std::string& message, bool is_signature) 
{
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int len = 0;

    // 计算HMAC-SHA256哈希值
    // HMAC(EVP_sha256(), key.c_str(), key.length(),
    //      reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), result, &len);
    HMAC_CTX* ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha256(), nullptr);
    HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
    HMAC_Final(ctx, result, &len);
    HMAC_CTX_free(ctx);
    // 将二进制哈希值转换为std::string
    signature = base64_encode(result, len);
    return to_hex_string(result, len);
}

void Authorization::signature_encode(const unsigned char *data, size_t input_length)
{
    signature = base64_encode(data, input_length);
}

void Authorization::set_api_id(std::string id)
{
    api_id = id;
}

void Authorization::set_api_secret(std::string secret)
{
    api_secret = secret;
}

void Authorization::set_api_key(std::string key)
{
    api_key = key;
}


void Authorization::set_host(std::string host)
{
    this->host = host;
}

