#include "blur_gaussian.h"
#include "qmath.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    for(int x=0; x<size; ++x){
        for(int y=0; y<size; ++y){
            mask(x,y)=getGauss(x-radius,y-radius,sigma);
        }
    }
    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    
    double sigma_sq_x2 = 2 * pow(sigma,2);
    return qExp(-(pow(x,2) + pow(y,2)) / sigma_sq_x2) / sigma_sq_x2;
}

