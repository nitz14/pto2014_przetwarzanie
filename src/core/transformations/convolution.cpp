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

    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return newImage;
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
            int temp = A[x][y];
            C[x][y] = A[col - 1 - y][row - 1 - x];
            C[col - 1 - y][row - 1 - x] = temp;
        }
    }
    return C;
}
