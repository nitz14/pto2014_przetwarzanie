#include "bin_iterbimodal.h"

#include "conversion_grayscale.h"
#include "histogram_equalization.h"
#include "../histogram.h"

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img) :
    Transformation(img)
{
}

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationIterBimodal::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    int T = 128;
    int T_new,temp;
    T_new = temp = 0;

    while (true)
    {
        int T_left,T_right,i,j;
        T_left=T_right=i=j=0;

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                temp = qGray(image->pixel(x, y));

                if (temp < T){
                    T_left = T_left + temp;
                    i++;
                }else{
                    T_right = T_right + temp;
                    j++;
                }
            }
        T_new = ((T_left / i) + (T_right / j)) / 2;
        if (T_new != T)
            T = T_new;
        else
            break;
    }
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            int qvalue;
            QRgb pixel = image->pixel(x, y);
            qvalue = qGray(pixel);

            if (qvalue < T)
                newImage->setPixel(x, y, Qt::color0);
            else
                newImage->setPixel(x, y, Qt::color1);
        }
    }

    return newImage;
}



