#ifndef _ALGO_HMAC_H_
#define _ALGO_HMAC_H_

#include <iostream>
#include <string>

std::string hmac_sha256(const std::string& key, const std::string& message);

#endif