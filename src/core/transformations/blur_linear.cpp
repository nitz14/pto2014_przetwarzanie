#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();
    int r = maskSize/2;
    r=r+1;
    int temp = 0;

    math::matrix<float> mask(maskSize, maskSize);

    for (int x=0;x<r;++x){
        for (int y=0;y<r;++y){
             mask(x,y)=tmpMask.at(temp).toDouble();
             temp++;
             if (normalize==1) mask(x,y)=mask(x,y)/sum(mask);
          }
    }

    return convolute(mask, RepeatEdge);
}
