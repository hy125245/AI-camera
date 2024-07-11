#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

// Converts binary data to a hex string
std::string to_hex_string(const unsigned char* data, size_t length) {
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return oss.str();
}

// Computes HMAC-SHA256
std::string hmac_sha256(const std::string& key, const std::string& message) {
    unsigned char* result;
    unsigned int len = SHA256_DIGEST_LENGTH;

    result = HMAC(EVP_sha256(), key.c_str(), key.length(),
                  reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), nullptr, nullptr);

    return to_hex_string(result, len);
}