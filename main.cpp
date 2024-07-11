#include <iostream>
#include <string>
#include <atomic>
#include <unistd.h>
#include <regex>
#include "sparkchain.h"
#include "sc_tts.h"
#include "API_parse.hpp"
#include "image_identify.hpp"
#include "tts.h"

using namespace SparkChain;
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        cout << "Usage: ./AI-carmer <config.json>" << endl;
        return -1;
    }
    cout << "\n######### llm Demo #########" << endl;
    // 全局初始化
    // int ret = initSDK(argv[1]);
    // if (ret != 0)
    // {
    //     cout << "initSDK failed:" << ret << endl;
    //     return -1;
    // }
    API_parse api_parse;
    api_parse.parse_json(argv[1]);
    Authorization authorization;
    authorization.set_api_secret(api_parse.api_secret);
    authorization.set_api_key(api_parse.api_key);
    authorization.set_host("spark-api.cn-huabei-1.xf-yun.com");
    string url = authorization.url_encode();
    cout << url << endl;
    //personateTTS("你好 世界"); // 超拟人合成
    
    // 退出
    //uninitSDK();
}



