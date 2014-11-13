#include "morph_erode.h"

MorphErode::MorphErode(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphErode::MorphErode(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphErode::morph(math::matrix<float> window, math::matrix<bool> se)
{
    float max=0.0;

    int tempx,tempy;
    tempx = window.rowno();
    tempy = window.colno();
    for(int i=0;i<tempx;i++){
        for(int j=0;j<tempy;j++){
            if(se(i,j)==true){
                if(window(i,j)>max){
                    max = window(i,j);
                }
            }
        }
    }
    return (int) max;
}
