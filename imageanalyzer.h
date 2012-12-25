#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QImage>
#include <QColor>

class ImageAnalyzerOptions {
public:
    ImageAnalyzerOptions() :
        cnt(15), h(20), s(10), l(10) {}
    ImageAnalyzerOptions(int cnt,int h, int s, int l) :
        cnt(cnt),h(h),s(s),l(l) {}
    int cnt;
    int h;
    int s;
    int l;
};

class ImageAnalyzer
{
public:
    ImageAnalyzer(const QImage &image, QSize scaledSize = QSize(200,200));


    QImage& scaled() {return m_scaled;}

    QList<QColor> analyze(const ImageAnalyzerOptions& options = ImageAnalyzerOptions());

protected:


    QImage m_scaled;

};

#endif // IMAGEANALYZER_H
