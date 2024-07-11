#if !defined(IMAGE_IDENTIFY_HPP)
#define IMAGE_IDENTIFY_HPP

#include "authorization.hpp"

class ImageIdentify : Authorization
{
private:
    /* data */
public:
    ImageIdentify(/* args */);
    ~ImageIdentify();
    void socket_init();
    


public:
    std::string RFC1123_time;

};



#endif // IMAGE_IDENTIFY_HPP
