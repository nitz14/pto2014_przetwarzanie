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

    EdgeLaplaceOfGauss* laplaceOfGauss = new EdgeLaplaceOfGauss(image, supervisor);
    int v0 = 128;
    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
        {
            math::matrix<float> mask = laplaceOfGauss->getMask(size, RepeatEdge);
            float min = 0;
            float max = 255;
            for (int x=0; x<size; ++x) {
                for(int y=0; y<size; ++y) {
                    if(mask(x,y) < min) {
                        min = mask(x,y);
                    }
                    if(mask(x,y) > max) {
                        max = mask(x,y);
                    }
                }
            }

            if(min<v0-t && max>v0+t) {
            } else {

            }

        }


    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return newImage;
}

