#include "imageanalyzer.h"

#define ARRAY_INDEX(a,b,c,a_s,b_s,c_s) (a+b*a_s+c*a_s*b_s)
#define COMMON_COLORS_COUNT 10
#define IMAGE_SIZE_CALCULATION 200
#define IMAGE_SIZE_PREVIEW 200
//#include <QSize>
#include <QImage>

ImageAnalyzer::ImageAnalyzer(const QImage &image)
{
    QSize scaledSize(IMAGE_SIZE_CALCULATION,IMAGE_SIZE_CALCULATION);
    m_scaled = image.scaled(scaledSize,Qt::KeepAspectRatio);
    analyze();
}

void ImageAnalyzer::analyze()
{
    int n = COMMON_COLORS_COUNT;

    int w = m_scaled.width();
    int h = m_scaled.height();

    // lets split the color cube to smaller cubes, n - number or groups, d - divisor
    int h_n,s_n,l_n,h_d,s_d,l_d;
    h_n = 15; //hue is most important I think
    s_n = 8;
    l_n = 8;
    h_d = 360 / h_n;
    s_d = 256 / s_n;
    l_d = 256 / l_n;

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
            int h,s,l,h_c,s_c,l_c;
            c.getHsl(&h,&s,&l);
            h_c = h / h_d; //category index
            s_c = s / s_d;
            l_c = l / l_d;
            categories[ARRAY_INDEX(h_c,s_c,l_c,h_n,s_n,l_n)]++;
        }

    for (int m=0;m<n;m++)
    {
        int c_max = 0;
        int h_max, s_max, l_max;
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

        categories[ARRAY_INDEX(h_max,s_max,l_max,h_n,s_n,l_n)] = 0;
        m_commonColors.append(QColor::fromHsl(h_max*h_d,s_max*s_d,l_max*l_d));
    }

}
