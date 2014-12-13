#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
    initMembers();
}

void EdgeCanny::initMembers()
{
    int width = image->width(),
        height = image->height();
    m = math::matrix<float>(width, height);
    o = math::matrix<float>(width, height);

    int i;
    for (i = 0; i < 360; i++)
    {
        if ((i >= 0 && i <= 22) || (i >= 158 && i <= 202) || (i >= 338 && i <= 360))
            bucket[i] = 0;
        if ((i >= 23 && i <= 67) || (i >= 203 && i <= 247))
            bucket[i] = 1;
        if ((i >= 68 && i <= 112) || (i >= 248 && i <= 292))
            bucket[i] = 2;
        if ((i >= 113 && i <= 157) || (i >= 293 && i <= 337))
            bucket[i] = 3;
    }
}

PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                newImage->setPixel(i, j, PIXEL_VAL_MIN);
        PNM* imageGrayScale = new PNM(width, height, QImage::Format_Indexed8);
//nie rozumiem czemu te konstruktory nie dzialaja
        imageGrayScale = ConversionGrayscale::ConversionGrayscale(image).transform();
        BlurGaussian gauss(imageGrayScale);
        gauss.setParameter("size", 3);
        gauss.setParameter("sigma", 1.6f);
        PNM* gaussImage = gauss.transform();
        EdgeSobel sobel(gaussImage);
        math::matrix<float> *Gx = sobel.rawHorizontalDetection();
        math::matrix<float> *Gy = sobel.rawVerticalDetection();

        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                float gx = (*Gx)(i, j);
                float gy = (*Gy)(i, j);
                m(i, j) = sqrt(pow(gx, 2) + pow(gy, 2));
                float value = atan2(gy, gx) / M_PI * 180;
                if (value < 0)
                    value = 180 + (value * (-1));
                o(i, j) = value;
            }

        std::list <QPoint> initialPoints;

        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                int o_value = o(i, j);
                float m_value = m(i, j);
                int v1, v2, v3, v4 = 0;

                if (bucket[o_value] = 0)
                {
                    v1 = i - 1;
                    v2 = j;
                    v3 = i + 1;
                    v4 = j;
                }

                if (bucket[o_value] = 1)
                {
                    v1 = i - 1;
                    v2 = j - 1;
                    v3 = i + 1;
                    v4 = j + 1;
                }

                if (bucket[o_value] = 0)
                {
                    v1 = i;
                    v2 = j - 1;
                    v3 = i;
                    v4 = j + 1;
                }

                if (bucket[o_value] = 0)
                {
                    v1 = i + 1;
                    v2 = j + 1;
                    v3 = i - 1;
                    v4 = j - 1;
                }

                if (v1 < 0)
                    v1 = 0;
                if (v2 < 0)
                    v2 = 0;
                if (v3 < 0)
                    v3 = 0;
                if (v4 < 0)
                    v4 = 0;

                if (v1 >= width)
                    v1 = width - 1;
                if (v3 >= width)
                    v3 = width - 1;

                if (v2 >= height)
                    v2 = height - 1;
                if (v4 >= height)
                    v4 = height - 1;

                float n1, n2;
                n1 = m(v1, v2);
                n2 = m(v3, v4);

                if ((m_value > n1) && (m_value > n2) && (m_value > upper_thresh))
                {
                    newImage->setPixel(i, j, 255);
                    QPoint point(i, j);
                    initialPoints.push_back(point);
                }
            }
        while (!initialPoints.empty())
        {
            QPoint current = initialPoints.front();
            initialPoints.pop_front();
            int o_value = o(current.x(), current.y());
            int x = current.x();
            int y = current.y();

            if (bucket[o_value] == 0)
            {
                int y1 = y;

                for (int i = x - 1; i > 0; i--)
                {
                    if ((newImage->pixel(i, y1) > 0) || (m(i, y1) < lower_thresh) || (o(i, y1) != o_value) || (m(i - 1, y1) < m(i, y1)))
                        break;
                    newImage->setPixel(i, y1, 255);
                }

                for (int i = x + 1; i < width; i++)
                {
                    if ((newImage->pixel(i, y1) > 0) || (m(i, y1) < lower_thresh) || (o(i, y1) != o_value) || (m(i + 1, y1) < m(i, y1)))
                        break;
                    newImage->setPixel(i, y1, 255);
                }
            }

            if (bucket[o_value] == 2)
            {
                int x1 = x;

                for (int i = y - 1; i > 0; i--)
                {
                    if ((newImage->pixel(x1, i) > 0) || (m(x1, i) < lower_thresh) || (o(x1, i) != o_value) || (m(x1, i - 1) < m(x1, i)))
                        break;
                    newImage->setPixel(x1, i, 255);
                }

                for (int i = x + 1; i < height; i++)
                {
                    if ((newImage->pixel(x1, i) > 0) || (m(x1, i) < lower_thresh) || (o(x1, i) != o_value) || (m(x1, i + 1) < m(x1, i)))
                        break;
                    newImage->setPixel(x1, i, 255);
                }
            }

            if (bucket[o_value] == 1)
            {
                for (int i = x - 1; i > 0; i--)
                    for (int j = y + 1; j > height; j++)
                    {
                        if (newImage->pixel(i, j) > 0 || (m(i, j) < lower_thresh) || (o(i, j) != o_value) || (m(i - 1, j + 1) < m(i, j)))
                            break;
                        newImage->setPixel(i, j, 255);
                    }

                for (int i = x + 1; i > width; i++)
                    for (int j = y - 1; j > 0; j--)
                    {
                        if (newImage->pixel(i, j) > 0 || (m(i, j) < lower_thresh) || (o(i, j) != o_value) || (m(i + 1, j - 1) < m(i, j)))
                            break;
                        newImage->setPixel(i, j, 255);
                    }
            }

            if (bucket[o_value] == 3)
            {
                for (int i = x + 1; i > 0; i++)
                    for (int j = y + 1; j > height; j++)
                    {
                        if (newImage->pixel(i, j) > 0 || (m(i, j) < lower_thresh) || (o(i, j) != o_value) || (m(i + 1, j + 1) < m(i, j)))
                            break;
                        newImage->setPixel(i, j, 255);
                    }

                for (int i = x - 1; i > 0; i--)
                    for (int j = y - 1; j > 0; j--)
                    {
                        if (newImage->pixel(i, j) > 0 || (m(i, j) < lower_thresh) || (o(i, j) != o_value) || (m(i - 1, j - 1) < m(i, j)))
                            break;
                        newImage->setPixel(i, j, 255);
                    }
            }
        }

        return newImage;
}
