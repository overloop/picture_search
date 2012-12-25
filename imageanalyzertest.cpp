#include "imageanalyzertest.h"
#include "ui_imageanalyzertest.h"

#include "imageanalyzer.h"

#include <QPainter>

ImageAnalyzerTest::ImageAnalyzerTest(QStringList paths, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageAnalyzerTest),
    m_paths(paths),
    m_index(-1)
{
    ui->setupUi(this);
    on_next_clicked();
}

ImageAnalyzerTest::~ImageAnalyzerTest()
{
    delete ui;
}

void ImageAnalyzerTest::on_next_clicked()
{
    m_index++;
    int paths_size = m_paths.size();
    QString path = m_paths.at(m_index % paths_size);

    QList<ImageAnalyzerOptions> options;
    for (int i=8;i<20;i++)
    {
        options << ImageAnalyzerOptions(10,i*2,i,i);
    }

    QImage image(path);
    ImageAnalyzer analyzer(image);

    int w = 400;
    int h = 400;

    QImage palete(w,h,QImage::Format_RGB32);
    QPainter painter(&palete);

    int n = options.size();
    for (int i=0;i<n;i++)
    {
        QList<QColor> common = analyzer.analyze(options.at(i));
        int m = common.size();
        for (int j=0;j<m;j++)
        {
            QRect rect(i*w/n,j*h/m,w/n,h/n);
            painter.fillRect(rect,common.at(j));
        }
    }

    ui->image->setPixmap(QPixmap::fromImage(image.scaled(QSize(w,h),Qt::KeepAspectRatio)));
    ui->colors->setPixmap(QPixmap::fromImage(palete));
}
