#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QImage>

class ColorExtractrorOptions {
public:
    ColorExtractrorOptions() :
        cnt(15), h(20), s(10), l(10) {}
    ColorExtractrorOptions(int cnt,int h, int s, int l) :
        cnt(cnt),h(h),s(s),l(l) {}
    int cnt;
    int h;
    int s;
    int l;
};

class ColorExtractor
{
public:
    ColorExtractor(const QImage &image, QSize scaledSize = QSize(200,200));

    QImage& scaled() {return m_scaled;}

    virtual QList<QColor> extract(const ColorExtractrorOptions& options = ColorExtractrorOptions()) = 0;

protected:

    QImage m_scaled;
};

#endif // IMAGEANALYZER_H
