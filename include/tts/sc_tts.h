#ifndef SC_API_TTS
#define SC_API_TTS

#include <string>
#include <vector>
#include "sc_type.h"

namespace SparkChain {
using namespace std;


class SPARKCHAIN_API TTSResult {
public:
    virtual int seq() const = 0;
    virtual const char * data() const = 0;
    virtual size_t len() const = 0;
    virtual string ced() const = 0;
    virtual string pybuf() const = 0;
    virtual string version() const = 0;
    virtual int status() const = 0;
    virtual string sid() const = 0;
};

class SPARKCHAIN_API TTSError {
public:
    virtual int code() const = 0;
    virtual string errMsg() const = 0;
    virtual string sid() const = 0;
};

class SPARKCHAIN_API TTSCallbacks {
    public:
    virtual void onResult(TTSResult * result, void * usrTag = nullptr) = 0;
    virtual void onError(TTSError * error, void * usrTag = nullptr) = 0;
};
  
class SPARKCHAIN_API TTS {
public:
    virtual void vcn(string vcn);
   
    virtual void speed(int speed);
    virtual void pitch(int pitch);
    virtual void volume(int volume);

    virtual void reg(int reg);
    virtual void rdn(int rdn);

    virtual void sampleRate(int sampleRate);
    virtual void encoding(string encoding);
    virtual void bitDepth(int bitDepth);
    virtual void channels(int channels);
    virtual void frameSize(int frameSize);

    virtual void bgs(bool enable);
    virtual void rhy(bool enable);

    virtual void scn(int scn);
    virtual void version(bool enable);
    virtual void l5SilLen(int l5SilLen);
    virtual void paragraphSilLen(int paragraphSilLen);
    
    virtual int arun(string text);
    virtual int arun(string text, void * usrTag);
    virtual void stop();

    virtual void registerCallbacks(TTSCallbacks * cbs);
    
    explicit TTS(string vcn);
    virtual ~TTS();

    class Impl;
protected:
    TTS();
    void setImpl(Impl * Impl);
    Impl * pImpl;    
};

class SPARKCHAIN_API PersonateTTS : public TTS {
public:
    void topK(int topK);
    void maxTokens(int maxTokens);

    void oralLevel(string level);
    void sparkAssist(bool enable);

    explicit PersonateTTS(string vcn);
    ~PersonateTTS();

    // void arun(string text) override;
private:
    class Impl;
};
}


#endif