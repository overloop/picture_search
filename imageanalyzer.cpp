#include "imageanalyzer.h"

#define ARRAY_INDEX(a,b,c,a_s,b_s,c_s) (int(a+b*a_s+c*a_s*b_s))
#define COMMON_COLORS_COUNT 10

#define IMAGE_SIZE_PREVIEW 200
//#include <QSize>
#include <QImage>
#include <math.h>

#define round(a) (floor((a)+0.5))

ImageAnalyzer::ImageAnalyzer(const QImage &image, QSize scaledSize /*= QSize(IMAGE_SIZE_CALCULATION,IMAGE_SIZE_CALCULATION)*/)
{
    m_scaled = image.scaled(scaledSize,Qt::KeepAspectRatio);
    analyze();
}

QList<QColor> ImageAnalyzer::analyze(const ImageAnalyzerOptions& options/* = ImageAnalyzerOptions()*/)
{
    QList<QColor> colors;

    int n = options.cnt;

    int w = m_scaled.width();
    int h = m_scaled.height();

    // lets split the color cube to smaller cubes, n - number or groups, d - divisor
    int h_n,s_n,l_n;
    double h_nd,s_nd,l_nd;
    double h_d,s_d,l_d;
    h_n = options.h; //hue is most important I think
    s_n = options.s;
    l_n = options.l;

    h_nd = h_n;
    s_nd = s_n;
    l_nd = l_n;

    h_d = 1.0 / h_nd;
    s_d = 1.0 / s_nd;
    l_d = 1.0 / l_nd;

    int categories[h_n*s_n*l_n];
    for (int i=0;i<h_n;i++)
        for (int j=0;j<s_n;j++)
            for (int k=0;k<l_n;k++)
                categories[ARRAY_INDEX(i,j,k,h_n,s_n,l_n)] = 0;

    for(int i=0;i<h;i++)
        for(int j=0;j<w;j++)
        {
            QRgb cur = m_scaled.pixel(QPoint(j,i));
            QColor c = QColor::fromRgb(cur);
            int h_c,s_c,l_c;
            double h,s,l;
            c.getHslF(&h,&s,&l);
            h_c = floor(h / h_d); //category index
            s_c = floor(s / s_d);
            l_c = floor(l / l_d);
            categories[ARRAY_INDEX(h_c,s_c,l_c,h_n,s_n,l_n)]++;
        }

    for (int m=0;m<n;m++)
    {
        int c_max = 0;
        double h_max, s_max, l_max;
        for (int i=0;i<h_n;i++)
            for (int j=0;j<s_n;j++)
                for (int k=0;k<l_n;k++)
                {
                    int v = categories[ARRAY_INDEX(i,j,k,h_n,s_n,l_n)];
                    if (v>c_max)
                    {
                        c_max = v;
                        h_max = double(i)+0.5;
                        s_max = double(j)+0.5;
                        l_max = double(k)+0.5;
                    }
                }

        categories[ARRAY_INDEX(h_max,s_max,l_max,h_n,s_n,l_n)] = 0;
        colors.append(QColor::fromHslF(h_max*h_d,s_max*s_d,l_max*l_d));
    }

    return colors;
}
