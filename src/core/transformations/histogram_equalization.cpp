#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if(image->format() == QImage::Format_Indexed8){
        QHash<int, int>::const_iterator i;
        Histogram* hash = image->getHistogram();
        QHash<int, int> chan = *hash->get(Histogram::LChannel);
        double dis[256] = {0};
        i = chan.constBegin();
        while (i != chan.constEnd()){
            dis[i.key()] += i.value()/ width * height;
            ++i;
        }

        for(int j=1; j<256;++j){
            dis[j] += dis[j-1];
        }
        // blad !!
        for (int x=0; x<width; x++)
        {
            for (int y=0; y<height; y++)
            {
            QRgb pixel = image->pixel(x, y);
            newImage->setPixel(x, y, dis[qGray(pixel)] * 255);
            }
        }
    } else {
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
            double dis[256] = {0};
            i = chans.constBegin();
            while (i != chans.constEnd()){
                dis[i.key()] += i.value()/ width * height;
                ++i;
            }
            for(int j=1; j<256;++j){
                dis[j] += dis[j-1];
            }

            for (int x=0; x<width; x++)
            {
                for (int y=0; y<height; y++)
                {
                    QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                    int rgb[] = {qRed(pixel),qGreen(pixel),qBlue(pixel)};

                    //blad !!
                    for(int k=0; k<3; ++k) {
                        rgb[k] = dis[rgb[k]]*255;
                    }

                    newPixel = QColor(rgb[0], rgb[1], rgb[2]);
                    newImage->setPixel(x,y,newPixel.rgb());
                }
            }


        }
    }

    return newImage;
}

