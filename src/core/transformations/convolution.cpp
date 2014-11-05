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
            mask(x, y) = 0.0;
        }
    }
    mask(size/2, size/2) = 1.0;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    double weight = sum(mask);
    if (image->format() == QImage::Format_Indexed8){
        for (int x = 0; x < width; x++){
            for (int y = 0; y < height; y++){
                math::matrix<float> okno = getWindow(x, y, mask.rowno(), LChannel, mode);
                math::matrix<float> akumulator = join(okno, reflection(mask));
                float sum_aku = sum(akumulator);
                if (weight != 0){
                    sum_aku = sum_aku / weight;
                }
                if (sum_aku > 255){
                    sum_aku = 255;
                }
                if (sum_aku < 0){
                    sum_aku = 0;
                }
                newImage->setPixel(x, y, (int)floor(sum_aku));
            }
        }
    } else if (image->format() == QImage::Format_RGB32){
        for (int x = 0; x < width; x++){
            for (int y = 0; y < height; y++){
                math::matrix<float> okno = getWindow(x, y, mask.rowno(), RChannel, mode);
                math::matrix<float> akumulator = join(okno, reflection(mask));
                float sum_aku_red = sum(akumulator);
                okno = getWindow(x, y, mask.rowno(), GChannel, mode);
                akumulator = join(okno, reflection(mask));
                float sum_aku_green = sum(akumulator);

                okno = getWindow(x, y, mask.rowno(), BChannel, mode);
                akumulator = join(okno, reflection(mask));
                float sum_aku_blue = sum(akumulator);
                if (weight != 0)
                {
                    sum_aku_red = sum_aku_red / weight;
                    sum_aku_green = sum_aku_green / weight;
                    sum_aku_blue = sum_aku_blue / weight;
                }
                if (sum_aku_red > 255) {
                    sum_aku_red = 255;
                }
                if (sum_aku_red < 0){
                    sum_aku_red = 0;
                }
                if (sum_aku_green > 255) {
                    sum_aku_green = 255;
                }
                if (sum_aku_green < 0){
                    sum_aku_green = 0;
                }
                if (sum_aku_blue > 255) {
                    sum_aku_blue = 255;
                }
                if (sum_aku_blue < 0){
                    sum_aku_blue = 0;
                }
                QColor new_value = QColor((int)floor(sum_aku_red), (int)floor(sum_aku_green), (int)floor(sum_aku_blue));
                newImage->setPixel(x, y, new_value.rgb());
            }
        }
    }
    return newImage;
}

const int Convolution::convChannel(math::matrix<float> mask, int x, int y, int size, Channel channel, Mode mode) {
    float weight = sum(mask);
    math::matrix<float> win = getWindow(x,y, size, channel, mode);
    math::matrix<float> temp = this->join(win, this->reflection(mask));
    float suma = sum(temp);
    if(weight != 0) {
        suma = (float)suma / weight;
    }
    if(suma > 255) {
        suma = 255;
    } else if(suma < 0) {
        suma = 0;
    }
    return (int)suma;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for(int x=0; x<size; x++){
            for(int y=0; y<size; y++) {
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
    for(int x=0; x<row; x++){
            for(int y=0; y<col; y++){
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
            C((size - 1 - x), (size - 1 - y)) = A(x, y);
        }
    }
    return C;
}
