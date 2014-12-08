#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::transform()
{
    int thetaDensity = getParameter("theta_density").toInt();

    int width  = image->width(),
        height = image->height();

    PNM* tmpImage = ConversionGrayscale(image).transform();

    if(getParameter("skip_edge_detection").toBool() == false){
        EdgeLaplacian lapl(tmpImage);
        lapl.setParameter("size", 3);
        tmpImage = lapl.transform();
    }
    int rhoMax = sqrt(width*width+height*height);
    int thetaSize = 180*thetaDensity;
    PNM* newImage = new PNM(thetaSize, rhoMax*2+1, QImage::Format_Indexed8);
    math::matrix<int> hough(thetaSize, rhoMax*2+1);
    for(int x=hough.rowno()-1; x>=0; x--){
        for(int y=hough.colno()-1; y>=0; y--){
            hough(x,y) = 0;
        }
    }
    for(int i=0; i < width; i++){
        for(int j=0; j < height; j++){
            int v = qRed(tmpImage->pixel(i, j));
            if( v > 0){
                for(int k=0; k < thetaSize; k++){
                    double theta = (k*M_PI) /(180.0*thetaDensity);
                    int rho = round(i*cos(theta) + j*sin(theta));
                    hough(k, rho+rhoMax)++;
                }
            }
        }
    }
    int max = 0;
    for(int x=hough.rowno()-1; x>=0; x--){
        for(int y=hough.colno()-1; y>=0; y--){
            if(hough(x,y) > max)
                max = hough(x,y);
        }
    }
    for(int i=0; i < newImage->width(); i++){
        for(int j=0; j < newImage->height(); j++){
            newImage->setPixel(i, j, round(255*hough(i,j)/max));
        }
    }

    return newImage;
}
