#include "edge_laplacian_of_gauss.h"

#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();

    math::matrix<float> mask(size, size);

    for (int x=0; x<size; x++)
    {
        for (int y=0; y<size; y++)
        {
            mask(x,y) = getLoG(x - size/2, y - size/2, sigma);
        }
    }

    return mask;
}

float EdgeLaplaceOfGauss::getLoG(int x, int y, float s)
{
    return ((pow(x,2)+pow(y,2))/pow(s,2) - 2/pow(s,2)) * BlurGaussian::getGauss(x, y, s);
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}
