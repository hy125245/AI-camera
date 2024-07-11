#include <iostream>
#include <string>
#include <atomic>
#include <unistd.h>
#include <regex>
#include "sparkchain.h"
#include "sc_tts.h"
#include "API_parse.hpp"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace SparkChain;
using namespace std;

// async status tag
static atomic_bool finish(false);
// result cache
string final_result = "";
static FILE * file = nullptr;

class PersonateCallbacks : public TTSCallbacks
{
    void onResult(TTSResult * result,void * usrTag) {
        // printf("ced:%s\n",result->ced().c_str());
        if (file == nullptr) {
            file = fopen("./audio.pcm", "a");
        }
        const char * data = result->data();
        fwrite(data, 1, result->len(), file);
        if (result->status() == 2) {
            fclose(file);
            file = nullptr;
            finish = true;
        }
    }
    void onError(TTSError * error, void * usrTag) {
        finish = true;
        printf("%d,%s\n",error->code(),error->errMsg().c_str());
    }
};

class SparkCallbacks : public LLMCallbacks
{
    void onLLMResult(LLMResult *result, void *usrContext)
    {
        if(result->getContentType() == LLMResult::TEXT){
            int status = result->getStatus();
            printf(GREEN "%d:%s:%s:%s \n" RESET, status, result->getRole(), result->getContent(), usrContext);
            final_result += string(result->getContent());
            if (status == 2)
            {
                printf(GREEN "tokens:%d + %d = %d\n" RESET, result->getCompletionTokens(), result->getPromptTokens(), result->getTotalTokens());
                finish = true;
            }  
            
        }   
    }

    void onLLMEvent(LLMEvent *event, void *usrContext)
    {
        printf(YELLOW "onLLMEventCB\n  eventID:%d eventMsg:%s\n" RESET, event->getEventID(), event->getEventMsg());
    }

    void onLLMError(LLMError *error, void *usrContext)
    {
        printf(RED "onLLMErrorCB\n errCode:%d errMsg:%s \n" RESET, error->getErrCode(), error->getErrMsg());
        finish = true;  
    }
};

int initSDK(const std::string& file_name)
{
    // 全局初始化
    SparkChainConfig *config = SparkChainConfig::builder();
    API_parse api;
    api.parse_json(file_name);
    config->appID(api.api_id.c_str())        // 你的appid
        ->apiSecret(api.api_secret.c_str())   // 你的apisecret
        ->apiKey(api.api_key.c_str())        // 你的apikey
        ->workDir("./")
        ->logLevel(5); 
    int ret = SparkChain::init(config);
    printf(RED "\ninit SparkChain result:%d" RESET,ret);
    return ret;
}



void personateTTSTest(const char* txt)
{
    finish = false;
    PersonateTTS tts("x4_lingxiaoxuan_oral");
    // PersonateTTS tts("x4_lingfeizhe_oral");
    tts.sampleRate(24000);
    PersonateCallbacks *cbs = new PersonateCallbacks();
    tts.registerCallbacks(cbs);
    tts.arun(txt);
    int times = 0;
    while (!finish)
    { // 等待结果返回退出
        sleep(1);
        if (times++ > 20) // 等待十秒如果没有最终结果返回退出
            break;
    }
}

void uninitSDK()
{
    // 全局逆初始化
    SparkChain::unInit();
}