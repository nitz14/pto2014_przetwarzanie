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
    edgeImage = BinarizationGradient(edgeImage).transform();
    Hough hough(edgeImage);
    hough.setParameter("theta_density", 3);
    hough.setParameter("skip_edge_detection", true);
    PNM* tmpImage = hough.transform();

    QPainter painter(newImage);
    painter.setPen(QColor(255,0,0));

    for(int theta=1; theta < tmpImage->width(); theta++){
        for(int rho=0; rho < tmpImage->height(); rho++){
            if(qRed(tmpImage->pixel(theta, rho)) > threshold){
                //uzupelnic
            }
        }
    }
    //uzupelnic
    return newImage;
}
