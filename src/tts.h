#if !defined(TTS_H)
#define TTS_H

int initSDK(const std::string& file_name);
void personateTTSTest(const char* txt);
void uninitSDK();

#endif // TTS_H
