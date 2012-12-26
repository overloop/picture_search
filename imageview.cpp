#include "imageview.h"

#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QDebug>
#include <QPixmap>

ImageView::ImageView(QWidget *parent) :
    QAbstractItemView(parent),
    m_itemSize(150),
    m_columns(1)
{

}

void ImageView::paintEvent(QPaintEvent *event)
{
    //QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();
    //QStyle::State state = option.state;
    //QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));
    //QBrush highlight(option.palette.color(QPalette::Highlight));

    //p.fillRect(event->rect(),background);

    if (!model())
        return;

    QPainter p(viewport());

    int n = model()->rowCount();

    p.translate(-horizontalOffset(),-verticalOffset());

    for (int i=0;i<n;i++)
    {
        QModelIndex index = model()->index(i,0);

        QRect rect = itemRect(index);
        p.setPen(foreground);
        //p.drawRect(rect);

        QVariant decoration = model()->data(index,Qt::DecorationRole);
        if (decoration.type() == QVariant::Pixmap)
        {
            QPixmap pixmap = qvariant_cast<QPixmap>(decoration);
            if (!pixmap.isNull())
            {
                QSize pixmapSize = pixmap.size();
                QSize itemSize = rect.size();
                pixmapSize.scale(itemSize,Qt::KeepAspectRatio);
                QSize rest = (itemSize - pixmapSize)/2;
                QRect pixmapRect(QPoint(rest.width(),rest.height()),pixmapSize);
                pixmapRect.translate(rect.topLeft());
                p.drawPixmap(pixmapRect,pixmap);
            }

        }

        if (index == currentIndex())
        {
            QColor color(option.palette.color(QPalette::Highlight));
            color.setAlpha(100);
            p.fillRect(rect,color);
        }

        //qDebug() << rect << foreground.color() << background.color();
        //p.drawText(rect,Qt::AlignCenter,QString::number(i));
    }

    event->accept();
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    updateGeometries();
}

void ImageView::updateGeometries()
{
    QSize size = this->size();
    int columns = qMax(1,size.width() / m_itemSize);
    if (columns != m_columns && model())
    {
        m_columns = columns;
        repaint();
    }

    verticalScrollBar()->setPageStep(m_itemSize / 2);
    int rows = this->rows();
    int height = rows * m_itemSize;

    verticalScrollBar()->setRange(0,qMax(0,height-size.height()));
}

int ImageView::rows() const
{
    if (!model())
        return 0;
    int total = model()->rowCount();
    int rows = total / m_columns;
    if ((total % m_columns) == 0)
        return rows;
    else
        return rows+1;
}

QRect ImageView::itemRect(const QModelIndex& item) const
{
    //QSize size = this->size();
    int r = item.row();

    int row, col;
    row = r / m_columns;
    col = r % m_columns;

    return QRect(col*m_itemSize,row*m_itemSize,m_itemSize,m_itemSize);
}

QRect ImageView::visualRect(const QModelIndex &index) const
{
    QRect rect = itemRect(index);
    if (rect.isValid())
        return QRect(rect.left() - horizontalScrollBar()->value(),
                     rect.top() - verticalScrollBar()->value(),
                     rect.width(), rect.height());
    else
        return rect;
}

void ImageView::scrollTo(const QModelIndex &index, ScrollHint)
{
    QRect area = viewport()->rect();
    QRect rect = visualRect(index);

    if (rect.left() < area.left())
        horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + rect.left() - area.left());
    else if (rect.right() > area.right())
        horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + qMin(
                        rect.right() - area.right(), rect.left() - area.left()));

    if (rect.top() < area.top())
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + rect.top() - area.top());
    else if (rect.bottom() > area.bottom())
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + qMin(
                        rect.bottom() - area.bottom(), rect.top() - area.top()));

    update();
}

QModelIndex ImageView::indexAt(const QPoint &point) const
{
    if (!model())
        return QModelIndex();
    // Transform the view coordinates into contents widget coordinates.
    int wx = point.x() + horizontalScrollBar()->value();
    int wy = point.y() + verticalScrollBar()->value();

    int rows = model()->rowCount();

    QRect rect = itemRect(model()->index(0,0));
    int itemWidth = rect.width();
    int itemHeight = rect.height();

    int nx = wx / itemWidth;
    int ny = wy / itemHeight;

    if (nx>m_columns)
        return QModelIndex();

    int c = ny*m_columns+nx;
    if (c>rows)
        return QModelIndex();

    return model()->index(c,0);
}

int ImageView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int ImageView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool ImageView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

QRegion ImageView::visualRegionForSelection(const QItemSelection &selection) const
{
    int ranges = selection.count();

    if (ranges == 0)
        return QRect();

    QRegion region;
    for (int i = 0; i < ranges; ++i) {
        QItemSelectionRange range = selection.at(i);
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int col = range.left(); col <= range.right(); ++col) {
                QModelIndex index = model()->index(row, col,rootIndex());
                region += visualRect(index);
            }
        }
    }
    return region;
}

void ImageView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    // Use content widget coordinates because we will use the itemRegion()
    // function to check for intersections.

    QRect contentsRect = rect.translated(
                horizontalScrollBar()->value(),
                verticalScrollBar()->value()).normalized();

    int rows = model()->rowCount(rootIndex());
    int columns = model()->columnCount(rootIndex());
    QModelIndexList indexes;

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            QModelIndex index = model()->index(row, column, rootIndex());
            QRegion region = itemRect(index);
            if (!region.intersect(contentsRect).isEmpty())
                indexes.append(index);
        }
    }

    if (indexes.size() > 0) {
        int firstRow = indexes[0].row();
        int lastRow = indexes[0].row();
        int firstColumn = indexes[0].column();
        int lastColumn = indexes[0].column();

        for (int i = 1; i < indexes.size(); ++i) {
            firstRow = qMin(firstRow, indexes[i].row());
            lastRow = qMax(lastRow, indexes[i].row());
            firstColumn = qMin(firstColumn, indexes[i].column());
            lastColumn = qMax(lastColumn, indexes[i].column());
        }

        QItemSelection selection(
                    model()->index(firstRow, firstColumn, rootIndex()),
                    model()->index(lastRow, lastColumn, rootIndex()));
        selectionModel()->select(selection, command);
    } else {
        QModelIndex noIndex;
        QItemSelection selection(noIndex, noIndex);
        selectionModel()->select(selection, command);
    }

    update();
}

QModelIndex ImageView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers)
{
    QModelIndex current = currentIndex();

    int source_row = current.row();

    QModelIndex index;

    switch (cursorAction) {
    case MoveLeft:
        index = model()->index(source_row-1,0);
        if (index.isValid())
            current = index;
        break;
    case MoveUp:
        index = model()->index(source_row-m_columns,0);
        if (index.isValid())
            current = index;
        break;
    case MoveRight:
        index = model()->index(source_row+1,0);
        if (index.isValid())
            current = index;
        break;
    case MoveDown:
        index = model()->index(source_row+m_columns,0);
        if (index.isValid())
            current = index;
        break;
        break;
    default:
        break;
    }

    viewport()->update();
    return current;
}

