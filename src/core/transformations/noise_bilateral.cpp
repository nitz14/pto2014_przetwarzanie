#include "noise_bilateral.h"
#include <cmath>

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    sigma_d = getParameter("sigma_d").toInt();
    sigma_r = getParameter("sigma_r").toInt();
    radius = sigma_d;
    Mode mode = RepeatEdge;
    QRgb pixel;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            if(image->format() == QImage::Format_RGB32){
               pixel = getPixel(x,y,mode);
               int r,g,b;
               r=calcVal(x,y,RChannel);
               g=calcVal(x,y,GChannel);
               b=calcVal(x,y,BChannel);
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
               //sprawdzic
               QColor newPixel = QColor(r,g,b);
               newImage->setPixel(x,y,newPixel.rgb());
            }
            if(image->format() == QImage::Format_Mono){
               pixel = image->pixel(x,y);
               int v= qGrey(pixel);
               int temp = calcVal(x,y,LChannel);
               newImage->setPixel(x,y,temp);
            }
        }
    }
    return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
    Mode mode = RepeatEdge;
    int radius = sigma_d;
    math::matrix<double> win = get(x,y,radius*2+1,channel,mode);
    int licz,mian,temp,temp2,tempx,tempy;
    licz=mian=0;
    tempx = win.rowno();
    tempy = win.colno();
    for(int i=0;i<tempx;i++){
        for(int j=0;j<tempy;j){
            QPoint p1 = QPoint(i,j);
            QPoint p2 = QPoint(x,y);
            int val1,val2;
            //val1 = I(i,j);
            //val2 = I(x,y);
            temp=val1*colorCloseness(val1,val2)*spatialCloseness(p1,p2);
            temp2=colorCloseness(val1,val2)*spatialCloseness(p1,p2);
            licz+=temp;
            mian+=temp2;
        }
    }
    return licz/mian;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    return pow(M_E,-(pow(val1-val2,2.0)/2*pow(sigma_r,2.0)));
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
    return pow(M_E,-((pow(point1.x() - point2.x(),2.0)+pow(point1.y()-point2.y(),2.0))/2*pow(sigma_d,2.0)));
}
