#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);

    for(int x=0; x<size; ++x){
        for(int y=0; y<size; ++y){
            mask(x, y) = 0;
        }
    }
    mask(size/2, size/2) = 1;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    int size = mask.rowno();

    for(int x=0; x < image->width(); ++x) {
        for(int y=0; y < image->height(); ++y) {
            if(image->format() == QImage::Format_Indexed8) {
                int l = this->convChannel(mask, x, y, size, LChannel, mode);
                newImage->setPixel(x,y,l);
            } else {
                int r = this->convChannel(mask, x, y, size, RChannel, mode);
                int g = this->convChannel(mask, x, y, size, GChannel, mode);
                int b = this->convChannel(mask, x, y, size, BChannel, mode);
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
        }
    };

    return newImage;
}

const int Convolution::convChannel(math::matrix<float> mask, int x, int y, int size, Channel channel, Mode mode) {
    float weight = this->sum(mask);
    math::matrix<float> win = getWindow(x,y, size, channel, mode);
    math::matrix<float> temp = this->join(win, this->reflection(mask));
    float sum = this->sum(temp);
    if(weight != 0) {
        sum = (float)sum / weight;
    }
    if(sum > 255) {
        sum = 255;
    } else if(sum < 0) {
        sum = 0;
    }
    return (int)sum;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for(int x=0; x<size; ++x){
            for(int y=0; y<size; ++y) {
                C(x, y) = A(x, y) * B(x, y);
            }
        }
    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;
    int col = A.colno();
    int row  = A.rowno();
    for(int x=0; x<row; ++x){
            for(int y=0; y<col; ++y){
                sum += A(x,y);
            }
        }

    return sum;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);
    int col = A.colno();
    int row  = A.rowno();

    for (int x = 0; x < row; x++){
        for (int y = 0; y < col; y++){
            C(i,j) = A(size - i - 1, size - j - 1);
        }
    }
    return C;
}
