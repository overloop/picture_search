#ifndef IMAGEANALYZERNEUQUANT_H
#define IMAGEANALYZERNEUQUANT_H

#include "colorextractor.h"

class ColorExtractorNeuquant : public ColorExtractor
{
public:
    ColorExtractorNeuquant(const QImage &image, QSize scaledSize = QSize(200,200));

    virtual QList<QColor> extract(const ColorExtractrorOptions& options = ColorExtractrorOptions());
};

#endif // IMAGEANALYZERNEUQUANT_H
