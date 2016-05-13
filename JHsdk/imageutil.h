#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

class ImageUtil
{
public:
    ImageUtil();

    static void RAW8toRGB888(unsigned char * inBuf, int width, int height, unsigned char *rawImage);
    static void UYVYtoRGB888(unsigned char * inBuf, int width, int height, unsigned char *rawImage);
};

#endif // IMAGEUTIL_H
