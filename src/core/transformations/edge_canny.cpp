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
    float orientations[width][height];

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            m[x][y] = sqrt(pow((*horizGradient)(x,y),2) + pow((*vertiGradient)(x,y),2));
            orientations[x][y] = fmod(atan2((*vertiGradient)(x,y),(*horizGradient)(x,y))/M_PI,M_PI) / M_PI * 180.0;
        }
    }

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            int pixels_to_compare[2];

            switch (getGradientDirection(orientations[x][y])) {
            case 0:
                pixels_to_compare[0] = m[x+1][y];
                pixels_to_compare[1] = m[x-1][y];
                break;
            case 45:
                pixels_to_compare[0] = m[x+1][y-1];
                pixels_to_compare[1] = m[x-1][y+1];
                break;
            case 90:
                pixels_to_compare[0] = m[x][y+1];
                pixels_to_compare[1] = m[x][y-1];
                break;
            case 135:
                pixels_to_compare[0] = m[x-1][y-1];
                pixels_to_compare[1] = m[x+1][y+1];
                break;
            default:
                break;
            }

            if (m[x][y] > pixels_to_compare[0] && m[x][y] > pixels_to_compare[1] && m[x][y] > upper_thresh) {
                newImage->setPixel(x,y,255);
            } else {
                newImage->setPixel(x,y,0);
            }

        }
    }


    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            if(qGray(newImage->pixel(x,y)) == 255) {

            }
        }
    }

    return newImage;
}


int EdgeCanny::getGradientDirection(float orientation) {
    if (orientation >= 22.5 && orientation < 67.5 || orientation < -112.5 && orientation > -157.5) {
        return 45;
    } else if (orientation >= 67.5 && orientation < 112.5 || orientation < -67.5 && orientation > -112.5) {
        return 90;
    } else if (orientation >= 112.5 && orientation < 157.5 || orientation < -22.5 && orientation > -67.5) {
        return 135;
    } else if (orientation >= 157.5 && orientation < -157.5 || orientation >= -22.5 && orientation < 22.5) {
        return 0;
    }
}
