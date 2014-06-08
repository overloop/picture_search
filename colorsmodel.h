#ifndef COLORSMODEL_H
#define COLORSMODEL_H

#include <QAbstractTableModel>
#include <QColor>

class ColorsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ColorsModel(const QList<QColor>& colors, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    QList<QColor> m_colors;
    
signals:
    
public slots:
    
};

#endif // COLORSMODEL_H
