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
    int temp = 0;

    math::matrix<float> mask(maskSize, maskSize);

    for (int x = 0; x < maskSize; x++){
        for (int y = 0; y < maskSize; y++){
             mask(x,y)=tmpMask.at(temp).toDouble();
             temp++;
          }
    }

    if (normalize){
            float sum_mask = sum(mask);
            for (int x = 0; x < maskSize; x++){
                for (int y = 0; y < maskSize; y++){
                    mask(x, y) = mask(x, y) / sum_mask;
                }
            }
       }

    return convolute(mask, RepeatEdge);
}
