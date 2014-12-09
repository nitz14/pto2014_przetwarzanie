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
    float m[width][height];

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            m[x][y] = sqrt(pow((*horizGradient)(x,y),2) + pow((*vertiGradient)(x,y),2));
        }
    }

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            float orientation = fmod(atan2((*vertiGradient)(x,y),(*horizGradient)(x,y))/M_PI,M_PI) / M_PI * 180.0;

            int pixels_to_compare[2];
            if (orientation >= 22.5 && orientation < 67.5 || orientation < -112.5 && orientation > -157.5) {
                pixels_to_compare[0] = m[x+1][y-1];
                pixels_to_compare[1] = m[x-1][y+1];
            } else if (orientation >= 67.5 && orientation < 112.5 || orientation < -67.5 && orientation > -112.5) {
                pixels_to_compare[0] = m[x][y+1];
                pixels_to_compare[1] = m[x][y-1];
            } else if (orientation >= 112.5 && orientation < 157.5 || orientation < -22.5 && orientation > -67.5) {
                pixels_to_compare[0] = m[x-1][y-1];
                pixels_to_compare[1] = m[x+1][y+1];
            } else if (orientation >= 157.5 && orientation < -157.5 || orientation >= -22.5 && orientation < 22.5) {
                pixels_to_compare[0] = m[x+1][y];
                pixels_to_compare[1] = m[x-1][y];
            }

            if (m[x][y] > pixels_to_compare[0] && m[x][y] > pixels_to_compare[1] && m[x][y] > upper_thresh) {
                newImage->setPixel(x,y,Qt::white);
            } else {
                newImage->setPixel(x,y,Qt::black);
            }

        }
    }

    return newImage;
}
