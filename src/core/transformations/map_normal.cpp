#include "map_normal.h"

#include "edge_sobel.h"
#include "map_height.h"

MapNormal::MapNormal(PNM* img) :
    Convolution(img)
{
}

MapNormal::MapNormal(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* MapNormal::transform()
{
    int width  = image->width(),
        height = image->height();

    double strength = getParameter("strength").toDouble();

    PNM* newImage = new PNM(width, height, image->format());
    PNM* tempImage = MapHeight(image).transform();
    EdgeSobel sobel(tempImage);
    double dx , dy , dz, temp,len;
    math::matrix<double>* gx = sobel.rawHorizontalDetection();
    math::matrix<double>* gy = sobel.rawVerticalDetection();
    //sadze ze bez tego nizej bedzie dzialac tez
    newImage = new PNM(width,height,QImage::Format_RGB32);
    temp = 1/strength;
    for(int x=0; x<width;x++){
        for(int y=0; y<height;y++){
            dx = (*gx)(x,y);
            dy = (*gy)(x,y);
            len = sqrt(pow(dx,2.0)+pow(dy,2.0)+pow(temp,2.0));
            dx /= len;
            dy /= len;
            dz = temp/len;
            double temp2 =(255/strength);
            dx = (dx+1.0)*temp2;
            dy = (dy+1.0)*temp2;
            dz = (dz+1.0)*temp2;
            newImage->setPixel(x,y,qRgb(dx,dy,dz));
        }
    }
    return newImage;
}
