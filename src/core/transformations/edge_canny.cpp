#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);
    PNM* tmpImage = ConversionGrayscale(image).transform();
    BlurGaussian blurGauss(tmpImage);
    blurGauss.setParameter("size", 3);
    blurGauss.setParameter("sigma", 1.6);
    tmpImage = blurGauss.transform();
    EdgeSobel sobel(tmpImage);
    math::matrix<float>* horizGradient = sobel.rawHorizontalDetection();
    math::matrix<float>* vertiGradient = sobel.rawVerticalDetection();
    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            //cos tu jest
        }
    }

    return newImage;
}
