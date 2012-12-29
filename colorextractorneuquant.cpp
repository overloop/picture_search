#include "colorextractorneuquant.h"

#include "NEUQUANT.H"
#include <QColor>

ColorExtractorNeuquant::ColorExtractorNeuquant(const QImage &image, QSize scaledSize /*= QSize(200,200)*/) :
    ColorExtractor(image,scaledSize)
{

}

/*virtual*/
QList<QColor> ColorExtractorNeuquant::extract(const ColorExtractrorOptions &options)
{
    int sf = 5;

    int w = m_scaled.width();
    int h = m_scaled.height();
    bool needWordUnalign = (!((w % 4) == 0));

    Q_ASSERT(m_scaled.format() == QImage::Format_RGB32);

    uchar* image_data;
    if (!needWordUnalign)
    {
        image_data = m_scaled.scanLine(0);
    } else {
        image_data = (uchar*) malloc(w*h*3);
        for (int i=0;i<h;i++)
            memcpy(image_data+i*w*3,m_scaled.scanLine(i),w*3);
    }

    initnet(image_data,3*w*h,sf);
    learn();
    unbiasnet();
    uchar* colormap = (uchar*)malloc(netsize*3);
    writecolormap(colormap);

    QList<QColor> colors;
    for (int i=0;i<netsize;i++)
        colors.append(QColor::fromRgb(colormap[i*3],colormap[i*3+1],colormap[i*3+2]));

    if (needWordUnalign)
        free(image_data);
    free(colormap);

    return colors;
}
