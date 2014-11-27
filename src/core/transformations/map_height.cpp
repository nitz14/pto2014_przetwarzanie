#include "map_height.h"
#include "conversion_grayscale.h"

MapHeight::MapHeight(PNM* img) :
    Transformation(img)
{
}

MapHeight::MapHeight(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* MapHeight::transform()
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);
    //nie moze u mnie stworzyc takiego konstruktora ?o.O.
        newImage = ConversionGrayscale::ConversionGrayscale(image).transform();
    return newImage;
}
