#include "imageutil.h"

ImageUtil::ImageUtil()
{
}

// R G
// G B
void ImageUtil::RAW8toRGB888(unsigned char * inBuf, int width, int height, unsigned char *rawImage)
{
    ///fix last column
    for(int i=0; i<height; i++)
    {
        int idx = i*width+width-1;
        inBuf[idx] = inBuf[idx-2];
    }
    //first line
    for(int j=1; j<width-1; j++)
    {
        int idx = j;

        int r, g, b;
        if(j%2==0)
        {
            b = (inBuf[idx+width-1] + inBuf[idx+width+1])/2;
            g = (inBuf[idx+width]+inBuf[idx-1]+inBuf[idx+1])/3;
            r = inBuf[idx];
        }
        else
        {
            b = inBuf[idx+width];
            g = inBuf[idx];
            r = (inBuf[idx-1]+inBuf[idx+1])/2;
        }

        rawImage[3*idx] =  r;
        rawImage[3*idx+1] = g;
        rawImage[3*idx+2] = b;
    }

    //last line
    for(int j=1; j<width-1; j++)
    {
        int idx = (height-1)*width+j;

        int r, g, b;
        if(j%2==0)
        {
            b = (inBuf[idx-1]+inBuf[idx+1])/2;
            g = inBuf[idx];
            r = inBuf[idx-width];
        }
        else
        {
            b = inBuf[idx];
            g = (inBuf[idx-width]+inBuf[idx-1]+inBuf[idx+1])/3;
            r = (inBuf[idx-width-1] + inBuf[idx-width+1])/2;
        }

        rawImage[3*idx] =  r;
        rawImage[3*idx+1] = g;
        rawImage[3*idx+2] = b;
    }

    //first column
    for(int i=1; i<height-1; i++)
    {
        int idx = i*width+0;

        int r, g, b;
        if(i%2==0)
        {
            b = (inBuf[idx-width+1]+inBuf[idx+width+1])/2;
            g = (inBuf[idx-width]+inBuf[idx+width]+inBuf[idx+1])/3;
            r = inBuf[idx];
        }
        else
        {
            b = inBuf[idx+1];
            g = inBuf[idx];
            r = (inBuf[idx-width] + inBuf[idx+width])/2;
        }

        rawImage[3*idx] =  r;
        rawImage[3*idx+1] = g;
        rawImage[3*idx+2] = b;
    }

    //last column
    for(int i=1; i<height-1; i++)
    {
        int idx = i*width+width-1;

        int r, g, b;
        if(i%2==0)
        {
            b = (inBuf[idx-width]+inBuf[idx+width])/2;
            g = inBuf[idx];
            r = inBuf[idx-1];
        }
        else
        {
            b = inBuf[idx];
            g = (inBuf[idx-width]+inBuf[idx+width]+inBuf[idx-1])/3;
            r = (inBuf[idx-width-1] + inBuf[idx+width-1])/2;
        }

        rawImage[3*idx] =  r;
        rawImage[3*idx+1] = g;
        rawImage[3*idx+2] = b;
    }

//    //four corner
//    //TL B
//    int idx = 0;
//    rawImage[3*idx] =  inBuf[idx]; //b
//    rawImage[3*idx+1] = (inBuf[idx+1]+inBuf[idx+width])/2; //g
//    rawImage[3*idx+2] = inBuf[idx+width+1]; //r
//    //TR G
//    idx = width-1;
//    rawImage[3*idx] =  inBuf[idx-1]; //b
//    rawImage[3*idx+1] = inBuf[idx]; //g
//    rawImage[3*idx+2] = inBuf[idx+width]; //r
//    //BL G
//    idx = (height-1)*width;
//    rawImage[3*idx] =  inBuf[idx-width]; //b
//    rawImage[3*idx+1] = inBuf[idx]; //g
//    rawImage[3*idx+2] = inBuf[idx+1]; //r
//    //BR G
//    idx = (height-1)*width+width-1;
//    rawImage[3*idx] =  inBuf[idx-width-1]; //b
//    rawImage[3*idx+1] = (inBuf[idx-1]+inBuf[idx-width])/2; //g
//    rawImage[3*idx+2] = inBuf[idx]; //r

    //normal
    for(int i=1; i< height-1; i++)
    {
        for(int j=1; j<width-1; j++)
        {
            int idx = i*width + j;

            int r, g, b;
            if(j%2==0 && i%2==0)
            {
                b = (inBuf[idx-width-1] + inBuf[idx-width+1] + inBuf[idx+width-1] + inBuf[idx+width+1])/4;
                g = (inBuf[idx-width]+inBuf[idx+width]+inBuf[idx-1]+inBuf[idx+1])/4;
                r = inBuf[idx];
            }
            else if(j%2==1 && i%2==0)
            {
                b = (inBuf[idx-width]+inBuf[idx+width])/2;
                g = inBuf[idx];
                r = (inBuf[idx-1]+inBuf[idx+1])/2;
            }
            else if(j%2==0 && i%2==1)
            {
                b = (inBuf[idx-1]+inBuf[idx+1])/2;
                g = inBuf[idx];
                r = (inBuf[idx-width]+inBuf[idx+width])/2;
            }
            else
            {
                b = inBuf[idx];
                g = (inBuf[idx-width]+inBuf[idx+width]+inBuf[idx-1]+inBuf[idx+1])/4;;
                r = (inBuf[idx-width-1] + inBuf[idx-width+1] + inBuf[idx+width-1] + inBuf[idx+width+1])/4;
            }
            rawImage[3*idx] =  r;
            rawImage[3*idx+1] = g;
            rawImage[3*idx+2] = b;
        }
    }
}


void ImageUtil::UYVYtoRGB888(unsigned char * inBuf, int width, int height, unsigned char *rawImage)
{
    int len = width*height*2;
    for(int i=0; i< len/4; i++)
    {
        unsigned char u0 = inBuf[4*i];
        unsigned char y0 = inBuf[4*i+1];
        unsigned char v0 = inBuf[4*i+2];
        unsigned char y1 = inBuf[4*i+3];

        /*
        {
        int r = y0+8 + (1.402 * (v0-128));
        int g = y0 - (0.71414 * (v0-128)) - (0.34413 * (u0-128));
        int b = y0 + (1.772 * (u0-128));
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        rawImage[6*i] =  r; //r
        rawImage[6*i+1] = g; //g
        rawImage[6*i+2] = b; //b
        }

        {
        int r = y1+8 + (1.402 * (v0-128));
        int g = y1 - (0.71414 * (v0-128)) - (0.34413 * (u0-128));
        int b = y1 + (1.772 * (u0-128));
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        rawImage[6*i+3] =  r;
        rawImage[6*i+4] = g;
        rawImage[6*i+5] = b;
        }*/


        {
        int r = y0-16.0 + (1.370705 * (v0-128));
        int g = y0-16.0 - (0.698001 * (v0-128)) - (0.337633 * (u0-128));
        int b = y0-16.0 + (1.732446 * (u0-128));
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        rawImage[6*i] =  r; //r
        rawImage[6*i+1] = g; //g
        rawImage[6*i+2] = b; //b
        }

        {
        int r = y1-16.0 + (1.370705 * (v0-128));
        int g = y1-16.0 - (0.698001 * (v0-128)) - (0.337633 * (u0-128));
        int b = y1-16.0 + (1.732446 * (u0-128));
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        rawImage[6*i+3] =  r;
        rawImage[6*i+4] = g;
        rawImage[6*i+5] = b;
        }
    }
}
