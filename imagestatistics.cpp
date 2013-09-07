#include "imagestatistics.h"

ImageStatistics::ImageStatistics() {

}

ImageStatistics::ImageStatistics(const QString &file, const QString &preview, const QList<QColor> &colors) :
    file(file), preview(preview), colors(colors)
{

}

int ImageStatistics::h(int index) const {
    return colors.at(index).hue();
}

int ImageStatistics::s(int index) const {
    return colors.at(index).saturation();
}

int ImageStatistics::l(int index) const {
    return colors.at(index).value();
}
