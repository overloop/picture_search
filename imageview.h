#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QAbstractItemView>

/** @todo optimize paintEvent() - paint only visible images
 *  @todo split class on borrowed and original code (QNotSoAbstractItemView)
 *  @todo draw file name
 *  @todo implement show in file manager for unix and maybe mac (http://lynxline.com/show-in-finder-show-in-explorer/)
 *  @todo get rid of magic numbers 0 - pixmap 1 - path in model
 */

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

    void contextMenuEvent(QContextMenuEvent *event);

protected:

    int rows() const;

    QRect itemRect(const QModelIndex& item) const;
    int m_itemSize;
    int m_columns;
    QSize m_totalSize;

    QAction* m_showInFileManager;
    QAction* m_openInExternalViewer;
    QAction* m_copyPathToClipboard;

protected slots:

    void updateGeometries();
    void showInFileManager_triggered();
    void openInExternalViewer_triggered();
    void copyPathToClipboard_triggered();

signals:
    
public slots:
    
};

#endif // IMAGEVIEW_H
