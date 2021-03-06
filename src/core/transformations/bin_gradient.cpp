#include "bin_gradient.h"

BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    QRgb pixel;
    int top = 0;
    int bottom = 0;
    int gradientx = 0;
    int gradienty = 0;
    int gradient = 0;
    int temp = 0;

    for (int x=0; x<width; ++x)
    {
        for (int y=0; y<height; ++y)
        {
            QRgb pixel0 = image->pixel(x-1>=0 ? x-1 : x, y);
            QRgb pixel1 = image->pixel(x+1 < width ? x+1 : x, y);
            gradientx = qGray(pixel1) - qGray(pixel0);

            pixel0 = image->pixel(x, y-1>=0 ? y-1 : y);
            pixel1 = image->pixel(x, y+1<height ? y+1 : y);
            gradienty = qGray(pixel1) - qGray(pixel0);

            if (gradientx >= gradienty) {
                gradient = gradientx;
            }else{
                gradient = gradienty;
            }

            pixel = image->pixel(x, y);
            top += qGray(pixel) * gradient;
            bottom += gradient;
            temp = bottom==0 ? 0 : top / bottom;

            if (qGray(pixel) >= temp){
                newImage->setPixel(x, y, Qt::color1);
            }else{
                newImage->setPixel(x, y, Qt::color0);
            }
        }
    }

    return newImage;
}


