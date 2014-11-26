#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    g_x = math::matrix<float>(3,3);
    g_x(0,0) = -1;
    g_x(0,1) = 0;
    g_x(0,2) = 1;
    g_x(1,0) = -2;
    g_x(1,1) = 0;
    g_x(1,2) = 2;
    g_x(2,0) = -1;
    g_x(2,1) = 0;
    g_x(2,2) = 1;

    g_y = math::matrix<float>(3,3);
    g_y(0,0) = -1;
    g_y(0,1) = -2;
    g_y(0,2) = -1;
    g_y(1,0) = 0;
    g_y(1,1) = 0;
    g_y(1,2) = 0;
    g_y(2,0) = 1;
    g_y(2,1) = 2;
    g_y(2,2) = 1;
}

math::matrix<float>* EdgeSobel::rawHorizontalDetection()
{
    math::matrix<float>* x_gradient = new math::matrix<float>(this->image->width(), this->image->height());

    int width = x_gradient->rowno();
    int height = x_gradient->colno();
    for (int x=0; x<width; x++){
        for (int y=0; y<height; y++){
            math::matrix<float> window = getWindow(x, y, 3, LChannel, NullEdge);
            (*x_gradient)(x, y) = sum(join(g_x, window));
        }
    }
    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<float>* y_gradient = new  math::matrix<float>(this->image->width(), this->image->height());

    int width = y_gradient->rowno();
    int height = y_gradient->colno();
    for (int x=0; x<width; x++){
        for (int y=0; y<height; y++){
            math::matrix<float> window = getWindow(x, y, 3, LChannel, NullEdge);
            (*y_gradient)(x, y) = sum(join(g_y, window));
        }
    }
    return y_gradient;
}
