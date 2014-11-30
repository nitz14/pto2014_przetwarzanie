#include "map_horizon.h"

#include "map_height.h"

MapHorizon::MapHorizon(PNM* img) :
    Transformation(img)
{
}

MapHorizon::MapHorizon(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* MapHorizon::transform()
{
    int width  = image->width(),
        height = image->height();

    double scale     = getParameter("scale").toDouble();
    int    sun_alpha = getParameter("alpha").toInt();
    int dx, dy;

    switch (getParameter("direction").toInt())
    {
    case NORTH: dx = 0; dy = -1; break;
    case SOUTH: dx = 0; dy = 1; break;
    case EAST:  dx = 1; dy = 0; break;
    case WEST:  dx = -1; dy = 0; break;
    default:
        qWarning() << "Unknown direction!";
        dx =  0;
        dy = -1;
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    PNM* tempImage = MapHeight(image).transform();
    double alpha,dist,reyalpha,delta;
    int k,l;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            alpha = 0.0;
            int currenth = qRed(tempImage->pixel(x,y));
            //nie wiem czy to droga do slonca ?!
            for (k=x+dx,l=y+dy;k<width && l<height && k >= 0 && l >= 0; k+=dx,l+=dy){
                int rayh = qRed(tempImage->pixel(k,l));//tutaj nie bedzie x,y
                if(currenth < rayh){
                    dist = sqrt(pow(k-x,2.0)+pow(l-y,2.0)) * scale;
                    reyalpha = atan((rayh - currenth) / dist);
                    if(reyalpha > alpha){
                        alpha = reyalpha;
                    }
                }
            }
            delta = alpha - sun_alpha * 3.14/180.;
            if(delta > 0){
                newImage->setPixel(x,y,cos(delta)*255);
            }else{
                newImage->setPixel(x,y,255);
            }
        }
    }

    return newImage;
}
