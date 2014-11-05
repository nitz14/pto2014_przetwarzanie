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

    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            mask(x,y)=this->getGauss(x-radius,y-radius,sigma);
        }
    }
    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    
    return (1 / (2 * M_PI*sigma*sigma))*pow(M_E, (-1)*(x*x + y*y) / (2 * sigma*sigma));
}

