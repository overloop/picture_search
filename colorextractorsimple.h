#ifndef IMAGEANALYZERSIMPLEQUANT_H
#define IMAGEANALYZERSIMPLEQUANT_H

#include <QImage>
#include <QColor>

#include "colorextractor.h"

class ColorExtractorSimple : public ColorExtractor
{
public:
    ColorExtractorSimple(const QImage &image, QSize scaledSize = QSize(200,200));

    QList<QColor> extract(const ColorExtractrorOptions& options = ColorExtractrorOptions());

protected:

};

#endif // IMAGEANALYZERSIMPLEQUANT_H
