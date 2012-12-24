#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QImage>
#include <QColor>

class ImageAnalyzer
{
public:
    ImageAnalyzer(const QImage& image);

    QList<QColor> commonColors() const {return m_commonColors;}
    QImage& scaled() {return m_scaled;}

protected:

    void analyze();

    QList<QColor> m_commonColors;
    QImage m_scaled;

};

#endif // IMAGEANALYZER_H
