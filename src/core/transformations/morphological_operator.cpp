#include "morphological_operator.h"

MorphologicalOperator::MorphologicalOperator(PNM* img) :
    Transformation(img)
{
}

MorphologicalOperator::MorphologicalOperator(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

// abstract
const int MorphologicalOperator::morph(math::matrix<float>, math::matrix<bool>)
{
    return 0;
}

math::matrix<bool> MorphologicalOperator::getSE(int size, SE shape)
{
    switch (shape)
    {
    case Square:    return seSquare(size);
    case Cross:     return seCross(size);
    case XCross:    return seXCross(size);
    case VLine:     return seVLine(size);
    case HLine:     return seHLine(size);
    default:        return seSquare(size);
    }
}


math::matrix<bool> MorphologicalOperator::seSquare(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i=0; i<size;i++){
        for (int j=0;j<size;j++){
            ret(i,j)=true;
        }
    }
    return ret;
}

math::matrix<bool> MorphologicalOperator::seCross(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i=0; i<size;i++){
        for (int j=0;j<size;j++){
            if(i==size/2 || j == size/2){
                ret(i,j)=true;
            }else{
                ret(i,j)=false;
            }
        }
    }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seXCross(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i=0; i<size;i++){
        for (int j=0;j<size;j++){
            if(i==j || j == size-j){
                ret(i,j)=true;
            }else{
                ret(i,j)=false;
            }
        }
    }
    return ret;
}

math::matrix<bool> MorphologicalOperator::seVLine(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i=0; i<size;i++){
        for (int j=0;j<size;j++){
            if(i==size/2){
                ret(i,j)=true;
            }else{
                ret(i,j)=false;
            }
        }
    }
    return ret;
}

math::matrix<bool> MorphologicalOperator::seHLine(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i=0; i<size;i++){
        for (int j=0;j<size;j++){
            if(j == size/2){
                ret(i,j)=true;
            }else{
                ret(i,j)=false;
            }
        }
    }

    return ret;
}

PNM* MorphologicalOperator::transform()
{  
    int size  = getParameter("size").toInt();
    SE  shape = (MorphologicalOperator::SE) getParameter("shape").toInt();

    PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);

    int width = image->width();
    int height = image->height();
    QRgb pixel;
    math::matrix<bool> shapetemp = getSE(size,shape);

    if (image->format() == QImage::Format_RGB32){
        for (int i=0; i<width;i++){
            for (int j=0;j<height;j++){
               pixel = getPixel(i,j,RepeatEdge);
               double r=morph(getWindow(i,j,size,RChannel,RepeatEdge),shapetemp);
               double g=morph(getWindow(i,j,size,GChannel,RepeatEdge),shapetemp);
               double b=morph(getWindow(i,j,size,BChannel,RepeatEdge),shapetemp);
               QColor newPixel = QColor(r,g,b);
               newImage->setPixel(i,j,newPixel.rgb());
            }
        }
    }
    if (image->format() == QImage::Format_Mono){
        for (int i=0; i<width;i++){
            for (int j=0;j<height;j++){
                pixel = getPixel(i,j,RepeatEdge);
                double g = morph(getWindow(i,j,size,LChannel,RepeatEdge),shapetemp);
                newImage->setPixel(i,j,g);
            }
        }
    }

    return newImage;
}
