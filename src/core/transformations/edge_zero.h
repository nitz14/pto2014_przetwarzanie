#ifndef EDGE_ZERO_H
#define EDGE_ZERO_H

#include "convolution.h"

class EdgeZeroCrossing : public Convolution
{
public:
    EdgeZeroCrossing(PNM*);
    EdgeZeroCrossing(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    double findMin(math::matrix<double> mask);
    double findMax(math::matrix<double> mask);
};

#endif // EDGE_ZERO_H
