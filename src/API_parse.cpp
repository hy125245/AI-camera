#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "API_parse.hpp"

API_parse::API_parse(/* args */)
{

}

API_parse::~API_parse()
{

}

int API_parse::parse_json(const std::string& file_name)
{
    try {
        std::ifstream fd(file_name);
        if (!fd) {
            // 提供文件名和更详细的错误信息
            std::cerr << "Error: file not found - " << file_name << std::endl;
            return -1;
        }
        std::string line;
        // 使用RAII，作用域结束时文件自动关闭
        nlohmann::json json_obj;
        json_obj << fd;
        // 安全访问JSON元素，使用value方法并提供默认值
        // 假设默认值为"unknown"，根据实际情况调整
        this->api_id = json_obj["APPID"];
        this->api_secret = json_obj["APISECRET"];
        this->api_key = json_obj["APIKEY"];
    } catch (const std::exception& e) {
        // 捕获并处理所有std::exception派生的异常
        std::cerr << "Exception caught while parsing JSON: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        // 捕获所有未知异常
        std::cerr << "An unknown error occurred" << std::endl;
        return -1;
    }
    return 0;
}