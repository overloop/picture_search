#ifndef IMAGEANALYZERTEST_H
#define IMAGEANALYZERTEST_H

#include <QWidget>

namespace Ui {
    class ColorExtractorTest;
}

class ColorExtractorTest : public QWidget
{
    Q_OBJECT

public:
    explicit ColorExtractorTest(QStringList paths, QWidget *parent = 0);
    ~ColorExtractorTest();


private:
    Ui::ColorExtractorTest *ui;
    QStringList m_paths;
    int m_index;

    void drawPalette(const QList<QColor>& colors, QRect rect, QPainter* p);

private slots:
    void on_next_clicked();

};

#endif // IMAGEANALYZERTEST_H
