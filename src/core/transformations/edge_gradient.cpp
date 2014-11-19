#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    PNM* newImage = new PNM(image->width(), image->height(), image->format());

    PNM* imageX = horizontalDetection();
    PNM* imageY = verticalDetection();

    if (image->format() == QImage::Format_Indexed8)
    {
        // Iterate over image space
        for (int x=0; x<image->width(); x++)
            for (int y=0; y<image->height(); y++)
            {
                QRgb pixelX = imageX->pixel(x,y); // Getting the pixel(x,y) value
                QRgb pixelY = imageY->pixel(x,y); // Getting the pixel(x,y) value

                int v = (int) sqrt(pow(qGray(pixelX),2.0)+pow(qGray(pixelY),2.0));
                newImage->setPixel(x,y,v);
            }
    }
    else if (image->format() == QImage::Format_RGB32)
    {
        // Iterate over image space
        for (int x=0; x<image->width(); x++)
            for (int y=0; y<image->height(); y++)
            {
                QRgb pixelX = imageX->pixel(x,y); // Getting the pixel(x,y) value
                QRgb pixelY = imageY->pixel(x,y); // Getting the pixel(x,y) value

                int r = (int) sqrt(pow(qRed(pixelX), 2.0) + pow(qRed(pixelY), 2.0));
                int g = (int) sqrt(pow(qGreen(pixelX), 2.0) + pow(qGreen(pixelY), 2.0));
                int b = (int) sqrt(pow(qBlue(pixelX), 2.0) + pow(qBlue(pixelY), 2.0));

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }
    return newImage;
}

