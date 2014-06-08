#include "colorsview.h"

#include <QHeaderView>

ColorsView::ColorsView(QWidget *parent) :
    QTableView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}

void ColorsView::setModel(QAbstractItemModel *model) {

    QTableView::setModel(model);
    if (model) {
        resizeRowsToContents();
        resizeColumnsToContents();
        updateGeometry();
    }

}

QSize ColorsView::sizeHint() const {

    QAbstractItemModel* m = model();
    if (m) {
        QHeaderView* horHeader = horizontalHeader();
        QHeaderView* verHeader = verticalHeader();

        int rows = m->rowCount();
        int cols = m->columnCount();
        int x = horHeader->sectionViewportPosition(cols-1) + horHeader->offset()
                + horHeader->sectionSize(cols-1) + 1;
        int y = verHeader->sectionViewportPosition(rows-1) + verHeader->offset()
                + verHeader->sectionSize(rows-1) + 1;
        QPoint p = viewport()->mapToParent(QPoint(x,y));

        return QSize(p.x(),p.y());
    }

    return QTableView::sizeHint();
}

QSize ColorsView::minimumSizeHint() const {
    return QSize(5,5);
}
