#include "segmentation.h"

#include "conversion_grayscale.h"
#include "blur_gaussian.h"

#include <queue>

Segmentation::Segmentation(PNM* img) :
    Transformation(img)
{
}

Segmentation::Segmentation(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

#define INIT -1
#define MASK -2
#define WSHED 0

QPoint* Segmentation::neighbourhood(QPoint p)
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return new QPoint();
}

PNM* Segmentation::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* imageGrayScale = new PNM(width, height, QImage::Format_Indexed8);
    imageGrayScale = ConversionGrayscale(image).transform();

    int curlab = 0;
    QQueue<QPoint> queue;
    math::matrix<int> lab(width, height);
    math::matrix<int> dist(width, height);

    //SORT pixels in increasing order of grey values (minimu h_min, maximum h_max)
    int h_min = 255;
    int h_max = 0;

    for(int i=0; i < width; i++){
        for(int j=0; j < height; j++){
            labs(i,j) = INIT;
            dist(i,j) = 0;

            int pixel = qGray(image->pixel(i,j));

            if(pixel > h_max)
                h_max = pixel;
            if(pixel < h_min)
                h_min = pixel;
        }
    }

    // start flooding
    for(int h=h_min; h<=h_max; h++) {
        for(int i=0; i < width; i++){
            for(int j=0; j < height; j++){
                if(qGray(imageGrayScale->pixel(i,j)) == h) {
                    lab(i,j) = MASK;

                    for(k=-1;k<2;k++) {
                        for(l=-1;l<2;l++) {
                            if(i+k<0 || j+l<0)
                                continue;
                            if(lab(i+k,j+l)>0 || lab(i+k,j+l)==WSHED) {
                                dist(i,j) = 1;
                                queue.enqueue(QPoint(i,j));
                            }
                        }
                    }

                }
            }
        }
    }

    int curdist = 1;
    queue.enqueue(QPoint(-1,-1));

    // extend basins
    while (!queue.isEmpty()) {
        QPoint point = queue.dequeue();
        if(point.x==-1 and point.y==-1) {
            if(queue.isEmpty())
                break;
            queue.enqueue(QPoint(-1,-1));
            curdist++;
            point = queue.dequeue();

            // labelling p by inspecting neighbours
            for(k=-1;k<2;k++) {
                for(l=-1;l<2;l++) {
                    if(point.x+k<0 || point.y+l<0)
                        continue;
                    QPoint q(point.x+k,point.y+l);
                    if(dist(q.x,q.y) < curdist && lab(q.x,q.y) > 0 || lab(q.x,q.y) == WSHED ) {
                        if(lab(q.x,q.y) > 0) {
                            if(lab(point.x,point.y) == MASK || lab(point.x,point.y) == WSHED)
                                lab(point.x,point.y) = lab(q.x,q.y);
                            else if (lab(point.x,point.y) != lab(q.x,q.y))
                                lab(point.x, point.y) = WSHED;
                        } else if (lab(point.x,point.y) == MASK)
                            lab(point.x,point.y) == WSHED;

                    } else if (lab(q.x,q.y) == MASK && dist(q.x,q.y) == 0) {
                        dist(q.x,q.y) = curdist + 1;
                        queue.enqueue(q);
                    }
                }
            }
        }
    }

    // detect and process new minima at level h

    // koniec - linia 53 ...


    return 0;
}
