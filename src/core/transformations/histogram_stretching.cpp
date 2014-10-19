#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Indexed8)
    {
        QHash<int, int>::const_iterator i;
        QHash<int, int>* hash = image->getHistogram()->get(Histogram::LChannel);
        int min = 0;
        int max = 0;
        for (i = hash->constBegin(); i != hash->constEnd(); ++i) {
            if (i.value() != 0) {
                min = i.key();
                break;
            }
        }


    // tutaj chyba jest zle
        for (i = hash->constEnd(); i != hash->constBegin(); --i) {
            if (i.value() != 0) {
                max = i.key();
                break;
            }
        }
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = 255 / (max-min) * (qGray(pixel) - min);

                newImage->setPixel(x, y, v);
            }
    }
    else if (image->format() == QImage::Format_RGB32) {
//        for (int x=0; x<width; x++)
//            for (int y=0; y<height; y++)
//            {
//                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

//                int r = qRed(pixel);    // Get the 0-255 value of the R channel
//                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
//                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

//                R->insert(r, R->value(r, 0)+1);
//                G->insert(g, G->value(g, 0)+1);
//                B->insert(b, B->value(b, 0)+1);
//            }
    }

    return newImage;
}


