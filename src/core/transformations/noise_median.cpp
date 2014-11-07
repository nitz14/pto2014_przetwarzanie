#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());
    Mode mode = RepeatEdge;
    QRgb pixel;
    int r,g,b,v,tempv;
    for(int x=0; x<width;x++){
        for(int y=0; y<height;y++){
            if(image->format() == QImage::Format_RGB32){
                pixel = getPixel(x,y,mode);
                r=getMedian(x,y,RChannel);
                g=getMedian(x,y,GChannel);
                b=getMedian(x,y,BChannel);
                //te warunki sa bo cos nie pyklo w getMedian can you look ?
                if(r>255){
                    r=255;
                }
                if(r<0){
                    r=0;
                }
                if(g>255){
                    g=255;
                }
                if(g<0){
                    g=0;
                }
                if(b>255){
                    b=255;
                }
                if(b<0){
                    b=0;
                }
                //to mozna wywalic jak naprawimy to nizej
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y,newPixel.rgb());
            }
            if(image->format() == QImage::Format_Mono){
                pixel = image->pixel(x,y);
                v=qGray(pixel);
                tempv=getMedian(x,y,LChannel);
                newImage->setPixel(x,y,tempv);
            }
        }
    }
    return newImage;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();
    Mode mode =	RepeatEdge;
    int pom,temp;
    pom = radius*2+1;
    temp = 0;
    double tab[100];
    math::matrix<double> wind = getWindow(x,y,pom,channel,mode);
    int tempx,tempy;
    tempx= wind.colno();
    tempy= wind.rowno();
    for (int i=0; i<tempx;i++){
        for (int j=0; j<tempy;j++){
            tab[temp]=wind(i,j);
            temp++;
        }
    }
    std::sort(&tab[0],&tab[temp]);
    return tab[temp/2];
}
