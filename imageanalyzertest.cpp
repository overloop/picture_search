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
    for (int i=8;i<30;i=i+2)
    {
        int h = i*2;
        int s = i;
        int l = i;
        options << ImageAnalyzerOptions(15,h,s,l);
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
        ImageAnalyzerOptions opt = options.at(i);
        QList<QColor> common = analyzer.analyze(opt);
        int m = common.size();
        for (int j=0;j<m;j++)
        {
            QRect rect(i*w/n,j*h/m,w/n,h/m);
            QColor background = common.at(j);
            QColor foreground;

            if (qGray(background.rgb())>128)
                foreground = QColor(Qt::black);
            else
                foreground = QColor(Qt::white);

            painter.fillRect(rect,background);
            painter.setBrush(QBrush(foreground));
            switch(j)
            {
            case 0:
                painter.drawText(rect, Qt::AlignCenter, QString("h=%1").arg(opt.h));
                break;
            case 1:
                painter.drawText(rect, Qt::AlignCenter, QString("s=%1").arg(opt.s));
                break;
            case 2:
                painter.drawText(rect, Qt::AlignCenter, QString("l=%1").arg(opt.l));
                break;
            }

        }
    }

    ui->image->setPixmap(QPixmap::fromImage(image.scaled(QSize(w,h),Qt::KeepAspectRatio)));
    ui->colors->setPixmap(QPixmap::fromImage(palete));

}
