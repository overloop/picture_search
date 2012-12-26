
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QAbstractItemView>

class ImageView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);
    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint);
    QModelIndex indexAt(const QPoint &point) const;
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);

    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

protected:

    int rows() const;

    QRect itemRect(const QModelIndex& item) const;
    int m_itemSize;
    int m_columns;
    QSize m_totalSize;

protected slots:

    void updateGeometries();

signals:
    
public slots:
    
};

#endif // IMAGEVIEW_H
