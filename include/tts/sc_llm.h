#ifndef SC_LLM_API_H
#define SC_LLM_API_H

#include "sc_type.h"
#include "sc_memory.h"

namespace SparkChain {

/**
 * @brief LLMConfig
 * 
 */
class SPARKCHAIN_API LLMConfig {
public:
    static LLMConfig* builder();

    virtual ~LLMConfig();

    virtual LLMConfig* uid(const char* uid)           = 0;
    virtual LLMConfig* domain(const char* domain)     = 0;
    virtual LLMConfig* auditing(const char* auditing) = 0;
    virtual LLMConfig* chatID(const char* chatID)     = 0;
    virtual LLMConfig* temperature(float temperature) = 0;
    virtual LLMConfig* topK(int topK)                 = 0;
    virtual LLMConfig* maxToken(int maxToken)         = 0;
    virtual LLMConfig* url(const char* url)           = 0;
    
    virtual LLMConfig* param(const char* key, const char* value) = 0;
    virtual LLMConfig* param(const char* key, int value)         = 0;
    virtual LLMConfig* param(const char* key, double value)      = 0;
    virtual LLMConfig* param(const char* key, bool value)        = 0;

};

class SPARKCHAIN_API LLMBaseOutput {
public:
    virtual ~LLMBaseOutput();
    
    virtual const char* getSid() = 0;
};


class SPARKCHAIN_API LLMResult : virtual public LLMBaseOutput {
public:
    enum ContentType {
        TEXT,
        IMAGE
    };
    virtual ~LLMResult();

    virtual const char* getRaw() const     = 0;
    virtual const char* getRole() const    = 0;
    virtual const char* getContent() const  = 0;
    virtual const ContentType getContentType() const = 0;
    virtual const int getContentLen() const = 0;


    virtual const int getStatus() const = 0;
    virtual const int getCompletionTokens() const = 0;
    virtual const int getPromptTokens()     const = 0;
    virtual const int getTotalTokens()      const = 0;
};

class SPARKCHAIN_API LLMEvent : virtual public LLMBaseOutput {
public:
    virtual ~LLMEvent();

    virtual const int   getEventID() const  = 0;
    virtual const char* getEventMsg() const = 0;
};

class SPARKCHAIN_API LLMError : virtual public LLMBaseOutput {
public:
    virtual ~LLMError();

    virtual const int   getErrCode() const = 0;
    virtual const char* getErrMsg() const  = 0;
};

class SPARKCHAIN_API LLMSyncOutput : virtual public LLMResult , virtual public LLMError {
public:
    virtual ~LLMSyncOutput();

};

class SPARKCHAIN_API LLMCallbacks {
public:
    /**
     * @brief 
     * 
     * @param result 
     * @param usrContext 
     */
    virtual void onLLMResult(LLMResult* result, void* usrContext) = 0;

    /**
     * @brief 
     * 
     * @param event 
     * @param usrContext 
     */
    virtual void onLLMEvent(LLMEvent* event, void* usrContext) = 0;

    /**
     * @brief 
     * 
     * @param error 
     * @param usrContext 
     */
    virtual void onLLMError(LLMError* error, void* usrContext) = 0;
};

typedef enum{
    TEXT_GENERATION = 0,
    IMAGE_GENERATION,
    IMAGE_UNDERSTANDING
} ModelType;

/**
 * @brief LLM
 * 
 */
class SPARKCHAIN_API LLM {
public:
    /*[deprecated] 请使用LLMFactory替代*/
    static LLM* create(LLMConfig* config = nullptr, Memory* memory = nullptr);
    /*[deprecated] 请使用LLMFactory替代*/
    static void destroy(LLM* llm);

public:
    LLM(const ModelType& type = TEXT_GENERATION, LLMConfig* config = nullptr, Memory* memory = nullptr);
    ~LLM();

    int addSystemPrompt(const char* prompt);
    

    /**
     * @brief 
     * 
     * @param question 
     * @param ttl
     * @return LLMSyncOutput* 
     */
    LLMSyncOutput* run(const char* question, int ttl = 60);
    LLMSyncOutput* run(const char* question, const char* image, const int& len, int ttl = 60);

    /**
     * @brief 
     * 
     * @param cbs 
     */
    void registerLLMCallbacks(LLMCallbacks* cbs);

    /**
     * @brief 
     * 
     * @param question 
     * @param usrTag 
     * @return int 
     */
    int arun(const char* question, void* usrTag = nullptr);
    int arun(const char* question, const char* image, const int& len, void* usrTag = nullptr);

    int stop();

    void clearHistory();

private:
    class LLMImpl;
    LLMImpl* pImpl;
};

class SPARKCHAIN_API LLMFactory {
public:
    static LLM* textGeneration(LLMConfig* config = nullptr, Memory* memory = nullptr);
    static LLM* imageUnderstanding(LLMConfig* config = nullptr, Memory* memory = nullptr);
    static LLM* imageGeneration(int width = 512, int height = 512, LLMConfig* config = nullptr);
    
    static void destroy(LLM* llm);
};

}

#endif  //SC_LLM_API_H
