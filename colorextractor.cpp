#include "colorextractor.h"

ColorExtractor::ColorExtractor(const QImage &image, QSize scaledSize /*= QSize(200,200)*/)
{
    m_scaled = image.scaled(scaledSize,Qt::KeepAspectRatio).convertToFormat(QImage::Format_RGB32);

}
