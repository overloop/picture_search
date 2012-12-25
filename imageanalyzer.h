#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QImage>
#include <QColor>
#define IMAGE_SIZE_CALCULATION 200

class ImageAnalyzerOptions {
public:
    ImageAnalyzerOptions() :
        cnt(10), h(15), s(8), l(8) {}
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
    ImageAnalyzer(const QImage &image, QSize scaledSize = QSize(IMAGE_SIZE_CALCULATION,IMAGE_SIZE_CALCULATION));


    QImage& scaled() {return m_scaled;}

    QList<QColor> analyze(const ImageAnalyzerOptions& options = ImageAnalyzerOptions());

protected:


    QImage m_scaled;

};

#endif // IMAGEANALYZER_H
