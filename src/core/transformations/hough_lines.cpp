#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"
#include "morph_erode.h"

#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());

    EdgeLaplacian lapl(image);
    lapl.setParameter("size", 3);
    PNM* edgeImage = lapl.transform();
    PNM* binImage = BinarizationGradient(edgeImage).transform();

    Hough hough(edgeImage);
    hough.setParameter("theta_density", 3);
    hough.setParameter("skip_edge_detection", true);
    PNM* houghImage = hough.transform();
    QPainter p(newImage);
    p.setPen(Qt::red);

    for(int theta=0; theta < houghImage->width(); theta++) {
        for(int rho=0; rho < houghImage->height(); rho++) {
            if(qGray(houghImage->pixel(theta, rho)) > threshold) {
                double rtheta = ((double)theta/3.0)*M_PI/180.0;
                int rrho = rho - houghImage->height()/2;
                p.drawLine(0, round(rrho/sin(rtheta)), newImage->width()-1, round((rrho - (newImage->width()-1)*cos(rtheta))/sin(rtheta)));
            }
        }
    }
    if(drawWholeLines == false) {
        for(int x=0; x<binImage->width(); x++) {
            for(int y=0; y<binImage->height(); y++) {
                if(qGray(binImage->pixel(x,y)) == 0) {
                    newImage->setPixel(x,y, image->pixel(x,y));
                }
            }
        }
    }

    return newImage;
}
