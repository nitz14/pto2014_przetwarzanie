#include "conversion_grayscale.h"

ConversionGrayscale::ConversionGrayscale(PNM* img) :
    Transformation(img)
{
}

ConversionGrayscale::ConversionGrayscale(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* ConversionGrayscale::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    if (image->format() == QImage::Format_Mono)
    {
        // Iterate over image space
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QColor color = QColor::fromRgb(image->pixel(x,y)); // Getting the pixel(x,y) value
                newImage->setPixel(x,y, color == Qt::white ? PIXEL_VAL_MIN : PIXEL_VAL_MAX);
            }
    }
    else // if (image->format() == QImage::Format_RGB32)
    {
        // Iterate over image space
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel
                newImage->setPixel(x,y, (0.3*r + 0.6*g + 0.1*b));
            }
    }

    return newImage;
}
