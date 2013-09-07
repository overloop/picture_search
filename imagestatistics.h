#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include <QString>
#include <QColor>
#include <QMetaType>

class ImageStatistics
{
public:
    ImageStatistics();
    ImageStatistics(const QString& file,const QString& preview, const QList<QColor>& colors);

    int h(int index) const;
    int s(int index) const;
    int l(int index) const;

    QString file;
    QString preview;
    QList<QColor> colors;
};

Q_DECLARE_METATYPE(ImageStatistics);

#endif // IMAGESTATISTICS_H
