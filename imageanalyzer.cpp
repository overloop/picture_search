#include "imageanalyzer.h"

#include <QDebug>

#define ARRAY_INDEX(a,b,c,a_s,b_s,c_s) (a+b*a_s+c*a_s*b_s)

//#include <QSize>
#include <QImage>
#include <math.h>


ImageAnalyzer::ImageAnalyzer(const QImage &image, QSize scaledSize /*= QSize(IMAGE_SIZE_CALCULATION,IMAGE_SIZE_CALCULATION)*/)
{
    m_scaled = image.scaled(scaledSize,Qt::KeepAspectRatio);
}

QList<QColor> ImageAnalyzer::analyze(const ImageAnalyzerOptions& options/* = ImageAnalyzerOptions()*/)
{
    QList<QColor> colors;

    int n = options.cnt;

    int width = m_scaled.width();
    int height = m_scaled.height();

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

    h_d = 1.000001d / h_nd;
    s_d = 1.000001d / s_nd;
    l_d = 1.000001d / l_nd;

    int cat_size = h_n*s_n*l_n;
    int categories[cat_size];
    for (int i=0;i<h_n;i++)
        for (int j=0;j<s_n;j++)
            for (int k=0;k<l_n;k++)
                categories[ARRAY_INDEX(i,j,k,h_n,s_n,l_n)] = 0;

    double h,s,l;

    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            //qDebug() << m_scaled.width() << m_scaled.height();

            QRgb cur = m_scaled.pixel(QPoint(j,i));
            QColor c = QColor::fromRgb(cur);

            if (!c.isValid())
                continue;

            int h_c,s_c,l_c;
            c.getHslF(&h,&s,&l);
            h_c = int(h / h_d); //category index
            s_c = int(s / s_d);
            l_c = int(l / l_d);

            //qDebug() << h_c << s_c << l_c << h_n << s_n << l_n << h << s << l << h_d << s_d << l_d;
            int idx = ARRAY_INDEX(h_c,s_c,l_c,h_n,s_n,l_n);
            Q_ASSERT(idx < cat_size);
            categories[idx]++;
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
                        h_max = i;
                        s_max = j;
                        l_max = k;
                    }
                }
        int idx = ARRAY_INDEX(int(h_max),int(s_max),int(l_max),h_n,s_n,l_n);
        categories[idx] = 0;
        colors.append(QColor::fromHslF((h_max+0.5)*h_d,(s_max+0.5)*s_d,(l_max+0.5)*l_d));
    }

    return colors;
}
