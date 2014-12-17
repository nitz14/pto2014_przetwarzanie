#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    math::matrix<float> I_XX(width, height);
    math::matrix<float> I_YY(width, height);
    math::matrix<float> I_XY(width, height);
    math::matrix<float> corner_canditates(width, height);
    math::matrix<float> corner_nonmax_suppress(width, height);

    PNM* grayImage = ConversionGrayscale(image).transform();
    BlurGaussian gauss(grayImage);
    gauss.setParameter("size", 3);
    gauss.setParameter("sigma", 1.6f);
    PNM* gaussImage = gauss.transform();
    EdgeSobel sobel(gaussImage);
    math::matrix<float> *Gx = sobel.rawHorizontalDetection();
    math::matrix<float> *Gy = sobel.rawVerticalDetection();

    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
        {
            float gx = (*Gx)(i, j);
            float gy = (*Gy)(i, j);
            I_XX(i,j) = gx * gx;
            I_YY(i,j) = gy * gy;
            I_XY(i,j) = gx * gy;
        }

    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
        {
            if (i!=0 && j!=0) {

                float s_XX = 0.0;
                for(int k=-1; k<2; k++) {
                    for(int l=-1; l<2; l++) {
                        s_XX += I_XX(i+k,j+l) * BlurGaussian::getGauss(k,l,sigma);
                    }
                }
                s_XX /= sigma_weight;

                float s_YY = 0.0;
                for(int k=-1; k<2; k++) {
                    for(int l=-1; l<2; l++) {
                        s_YY += I_YY(i+k,j+l) * BlurGaussian::getGauss(k,l,sigma);
                    }
                }
                s_YY /= sigma_weight;

                float s_XY = 0.0;
                for(int k=-1; k<2; k++) {
                    for(int l=-1; l<2; l++) {
                        s_XY += I_XY(i+k,j+l) * BlurGaussian::getGauss(k,l,sigma);
                    }
                }
                s_XY /= sigma_weight;

                math::matrix<float> H(2, 2);
                H(0,0) = s_XX;
                H(0,1) = s_XY;
                H(1,0) = s_XY;
                H(1,1) = s_YY;
                float r = H.det() - k_param * pow(H.trace(), 2.0);

                if (r > threshold) {
                    corner_canditates(i,j) = r;
                }

            }
        }

    bool search = true;
    while (search) {
        search = false;
        for (int i=0; i<width; i++)
            for (int j=0; j<height; j++)
            {
                if (i!=0 && j!=0) {

                    bool isMax = true;
                    for(int k=-1; k<2; k++) {
                        for(int l=-1; l<2; l++) {
                            if(k!=0 && l!=0 && corner_canditates(i,j)<corner_canditates(i+k,j+l)) {
                                isMax = false;
                                break;
                            }
                        }
                        if (!isMax)
                            break;
                    }

                    if(isMax) {
                        corner_nonmax_suppress(i,j) = corner_canditates(i,j);
                    } else {
                        if (corner_canditates(i,j) > 0)
                            search = true;
                        corner_nonmax_suppress(i,j) = 0;
                    }
                }
            }
        corner_canditates = corner_nonmax_suppress;
    }

    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
        {
            if (corner_canditates(i,j) == 0)
                newImage->setPixel(i,j,Qt::color0);
            else
                newImage->setPixel(i,j,Qt::color1);
        }

    return newImage;
}

