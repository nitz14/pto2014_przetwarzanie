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

    EdgeLaplaceOfGauss edgeLaplaceOfGauss(image);
    PNM* newImage = edgeLaplaceOfGauss.transform();

    edgeLaplaceOfGauss.setParameter("size",size);
    edgeLaplaceOfGauss.setParameter("sigma",sigma);

    Transformation laplasian(edgeLaplaceOfGauss.transform());

    int temp = 128;

    if(image->format() == QImage::Format_Indexed8){
        for(int x=0; x<width; x++){
            for(int y=0; y<height;y++){
                math::matrix<float> mask = laplasian.getWindow(x,y,size,LChannel,RepeatEdge);
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
                math::matrix<float> redmask = laplasian.getWindow(x,y,size,RChannel,RepeatEdge);
                math::matrix<float> bluemask = laplasian.getWindow(x,y,size,BChannel,RepeatEdge);
                math::matrix<float> greenmask = laplasian.getWindow(x,y,size,GChannel,RepeatEdge);
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

double EdgeZeroCrossing::findMin(math::matrix<float> mask) {
    double min = 255;
    int width = mask.colsize();
    int height = mask.rowsize();
    for (int x=0; x<width; ++x) {
        for(int y=0; y<height; ++y) {
            if(mask(x,y) < min) {
                min = mask(x,y);
            }
        }
    }
    return min;
}

double EdgeZeroCrossing::findMax(math::matrix<float> mask) {
    double max = 0;
    int width = mask.colsize();
    int height = mask.rowsize();
    for (int x=0; x<width; ++x) {
        for(int y=0; y<height; ++y) {
            if(mask(x,y) > max) {
                max = mask(x,y);
            }
        }
    }
    return max;
}
