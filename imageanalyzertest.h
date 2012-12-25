#ifndef IMAGEANALYZERTEST_H
#define IMAGEANALYZERTEST_H

#include <QWidget>

namespace Ui {
    class ImageAnalyzerTest;
}

class ImageAnalyzerTest : public QWidget
{
    Q_OBJECT

public:
    explicit ImageAnalyzerTest(QStringList paths, QWidget *parent = 0);
    ~ImageAnalyzerTest();

private:
    Ui::ImageAnalyzerTest *ui;
    QStringList m_paths;
    int m_index;

private slots:
    void on_next_clicked();

};

#endif // IMAGEANALYZERTEST_H
