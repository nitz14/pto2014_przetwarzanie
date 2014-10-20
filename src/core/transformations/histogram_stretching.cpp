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
        Histogram* hash = image->getHistogram();
        QHash<int, int> chan = *hash->get(Histogram::LChannel);
        int min = 255;
        int max = 0;
        i = chan.constBegin();

        while (i != chan.constEnd()){
            int w = i.key();
            if(w > 0)
            {
                if (w < min) min = w;
                if (w > max) max = w;
            }
            ++i;
        }

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = 255 / (max-min) * (qGray(pixel) - min);

                newImage->setPixel(x, y, v);
            }
    }
    else {
        Histogram* hash = image->getHistogram();
        hash->get(Histogram::RChannel);
        QHash<int, int>::const_iterator i;
        QList<QHash<int, int>> chan;
        QHash<int, int> Red = *hash->get(Histogram::RChannel);
        QHash<int, int> Green = *hash->get(Histogram::GChannel);
        QHash<int, int> Blue = *hash->get(Histogram::BChannel);
        QColor newPixel;
        chan.append(Red);
        chan.append(Green);
        chan.append(Blue);
        int j = 0;

        for (QHash<int, int> chans: chan){
            int min = 255;
            int max = 0;
            i = chans.constBegin();
            //min max
            while (i != chans.constEnd()){
                int w = i.key();
                if(w > 0)
                {
                    if (w < min) min = w;
                    if (w > max) max = w;
                }
                ++i;
            }

            for (int x=0; x<width; ++x){
                for(int y=0; y>height; ++y){
                    int v;
                    QRgb pixel;
                    switch (j)
                    {
                    case 0 :
                        pixel = image->pixel(x,y);
                        v = 255 / (max-min) * (qGray(pixel) - min);
                        newPixel = QColor(v,qGreen(pixel),qBlue(pixel));
                        break;
                    case 1 :
                        pixel = image->pixel(x,y);
                        v = 255 / (max-min) * (qGray(pixel) - min);
                        newPixel = QColor(qRed(pixel),v,qBlue(pixel));
                        break;
                    case 2 :
                        pixel = image->pixel(x,y);
                        v = 255 / (max-min) * (qGray(pixel) - min);
                        newPixel = QColor(qRed(pixel),qGreen(pixel),v);
                        break;
                    default :
                        break;
                    }
                    newImage->setPixel(x,y,newPixel.rgb());
                }
            }
            ++j;
        }
    }

    return newImage;
}


