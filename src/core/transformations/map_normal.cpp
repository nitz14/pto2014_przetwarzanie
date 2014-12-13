#include "map_normal.h"

#include "edge_sobel.h"
#include "map_height.h"

MapNormal::MapNormal(PNM* img) :
    Convolution(img)
{
}

MapNormal::MapNormal(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* MapNormal::transform()
{
    int width  = image->width(),
        height = image->height();

    double strength = getParameter("strength").toDouble();

    PNM* newImage = new PNM(width, height, image->format());
    PNM* mapHeight_temp = MapHeight(image).transform();
    EdgeSobel sobel_object(mapHeight_temp);

    math::matrix<float>* Gx = sobel_object.rawHorizontalDetection();
    math::matrix<float>* Gy = sobel_object.rawVerticalDetection();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            double dxVector = (*Gx)(i, j) / PIXEL_VAL_MAX;
            double dyVector = (*Gy)(i, j) / PIXEL_VAL_MAX;
            double dzVector = 1 / strength;

            double dLength = sqrt(pow(dxVector, 2.0) + pow(dyVector, 2.0) + pow(dzVector, 2.0));
            dxVector = dxVector / dLength;
            dyVector = dyVector / dLength;
            dzVector = dzVector / dLength;

            dxVector = (dxVector + 1.0) * (PIXEL_VAL_MAX / strength);
            dyVector = (dyVector + 1.0) * (PIXEL_VAL_MAX / strength);
            dzVector = (dzVector + 1.0) * (PIXEL_VAL_MAX / strength);

            QColor newPixel = QColor(dxVector, dyVector, dzVector);
            newImage->setPixel(i, j, newPixel.rgb());
        }

    return newImage;
