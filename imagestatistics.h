#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include <QString>
#include <QColor>
#include <QMetaType>

class ImageStatistics
{
public:
    ImageStatistics();
    ImageStatistics(int id, const QString& file,const QString& preview, const QList<QColor>& colors = QList<QColor>());

    int id;
    QString file;
    QString preview;
    QList<QColor> colors;
};

class ImageStatisticsList : public QList<ImageStatistics> {

};

Q_DECLARE_METATYPE(ImageStatisticsList);

#endif // IMAGESTATISTICS_H
