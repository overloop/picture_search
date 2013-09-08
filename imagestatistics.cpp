#include "imagestatistics.h"

ImageStatistics::ImageStatistics() : id(-1) {

}

ImageStatistics::ImageStatistics(int id, const QString& file,const QString& preview, const QList<QColor>& colors) :
    id(id),file(file), preview(preview), colors(colors) {

}
