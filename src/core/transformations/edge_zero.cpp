#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}


PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();

    int    size  = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    EdgeLaplaceOfGauss edgeLaplaceOfGauss(image);
    edgeLaplaceOfGauss.setParameter("size",size);
    edgeLaplaceOfGauss.setParameter("sigma",sigma);

    Transformation laplasian(edgeLaplaceOfGauss.transform());

    int temp = 128;

    if(image->format() == QImage::Format_Indexed8){
        for(int x=0; x<width; x++){
            for(int y=0; y<height;y++){
                math::matrix<double> mask = laplasian.getWindow(x,y,size,LChannel,NullEdge);
                //tutaj szukanie maxa i minimum z mask
                double max = findMax(mask), min = findMin(mask);
                if(min < temp - t && max > temp + t){
                    int q = mask(size/2,size/2);
                    newImage->setPixel(x,y,q);
                }else{
                    newImage->setPixel(x,y,0);
                }
            }
        }
    }else{
        for(int x=0; x<width; x++){
            for(int y=0; y<height; y++){
                math::matrix<double> redmask = laplasian.getWindow(x,y,size,RChannel,NullEdge);
                math::matrix<double> bluemask = laplasian.getWindow(x,y,size,BChannel,NullEdge);
                math::matrix<double> greenmask = laplasian.getWindow(x,y,size,GChannel,NullEdge);
                //tutaj szukanie maxa i min dla kazdego
                double minR = findMin(redmask),maxR = findMax(redmask);
                double minB = findMin(bluemask),maxB = findMax(bluemask);
                double minG = findMin(greenmask),maxG = findMax(greenmask);
                int r,g,b;
                if(minR<temp-t &&maxR>temp+t){
                    r = redmask(size/2,size/2);
                }else{
                    r = 0;
                }
                if(minG<temp-t &&maxG>temp+t){
                    g = greenmask(size/2,size/2);
                }else{
                    g = 0;
                }
                if(minB<temp-t &&maxB>temp+t){
                    b = bluemask(size/2,size/2);
                }else{
                    b = 0;
                }
                newImage->setPixel(x,y,qRgb(r,g,b));
            }
        }
    }

    return newImage;

}

double EdgeZeroCrossing::findMin(math::matrix<double> mask) {
    double min = 255;
    for (int x=0; x<mask.colsize(); ++x) {
        for(int y=0; y<mask.rowsize(); ++y) {
            if(mask(x,y) < min) {
                min = mask(x,y);
            }
        }
    }
    return min;
}

double EdgeZeroCrossing::findMax(math::matrix<double> mask) {
    double max = 0;
    for (int x=0; x<mask.colsize(); ++x) {
        for(int y=0; y<mask.rowsize(); ++y) {
            if(mask(x,y) > max) {
                max = mask(x,y);
            }
        }
    }
    return max;
}
