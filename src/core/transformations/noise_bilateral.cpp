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
    int r,g,b;
    if(image->format() == QImage::Format_RGB32){
        for(int x=0;x<width;x++){
            for(int y=0;y<height;y++){
                pixel = getPixel(x,y,mode);

                r=calcVal(x,y,RChannel);
                g=calcVal(x,y,GChannel);
                b=calcVal(x,y,BChannel);
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y,newPixel.rgb());
            }
        }
    }
    else if(image->format() == QImage::Format_Indexed8){
        for(int x=0;x<width;x++){
            for(int y=0;y<height;y++){
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
    math::matrix<float> win = getWindow(x,y,radius*2+1,channel,mode);
    int tempx,tempy,temp;

    float licz, mian, temp1, temp2;
    licz=mian=0;
    tempx = win.rowno();
    tempy = win.colno();
    for(int i=0;i<tempx;i++){
        for(int j=0;j<tempy;j++){

            if(channel == RChannel){
                temp = qRed(image->pixel(x, y));
            }else if(channel == GChannel){
                temp = qGreen(image->pixel(x, y));
            }else if(channel == BChannel){
                temp = qBlue(image->pixel(x, y));
            }else{
                temp = qGray(image->pixel(x, y));
            }

            int px = x + (i - (int)((radius * 2 + 1) / 2));
            if (px <= 0) px = 0;
            if (px >= image->width()) px = image->width();
            int py = y + (j - (int)((radius * 2 + 1) / 2));
            if (py <= 0) py = 0;
            if (py >= image->height()) py = image->height();

            temp1 = win(i,j)*colorCloseness(win(i,j),temp)*spatialCloseness(QPoint(px, py), QPoint(x, y));
            temp2 = colorCloseness(win(i,j),temp)*spatialCloseness(QPoint(px, py), QPoint(x, y));
            licz+=temp1;
            mian+=temp2;
        }
    }
    return (int) licz/mian;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    return pow(M_E,-(pow(val1-val2,2.0)/(2*pow(sigma_r,2.0))));
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
    return pow(M_E,-((pow(point1.x() - point2.x(),2.0) + pow(point1.y() - point2.y(),2.0))/(2*pow(sigma_d,2.0))));
}
