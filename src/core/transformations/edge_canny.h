#ifndef EDGECANNY_H
#define EDGECANNY_H

#include "convolution.h"

class EdgeCanny : public Convolution
{
public:
    EdgeCanny(PNM*);
    EdgeCanny(PNM*, ImageViewer*);

    virtual PNM* transform();


private:
    void initMembers();
    short bucket[361];
    math::matrix<float> m, o;
};

#endif // EDGECANNY_H
