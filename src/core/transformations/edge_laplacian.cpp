#include "edge_laplacian.h"

EdgeLaplacian::EdgeLaplacian(PNM* img) :
    Convolution(img)
{
}

EdgeLaplacian::EdgeLaplacian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplacian::getMask(int, Mode)
{
    int size = getParameter("size").toInt();
    math::matrix<float> mask(size, size);

    for (int x=0; x<size; x++)
    {
        for (int y=0; y<size; y++)
        {
            mask(x,y) = -1;
        }
    }
    mask((int) size/2, (int) size/2) = size * size - 1;

    return mask;
}

