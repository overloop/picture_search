#include "colorextractortest.h"
#include "ui_colorextractortest.h"

#include "colorextractorsimple.h"
#include "colorextractorneuquant.h"

#include <QPainter>

void ColorExtractorTest::drawPalette(const QList<QColor>& colors, QRect rect, QPainter* p)
{
    int h = rect.height();
    //int w = rect.width();

    int n = colors.size();
    int ch = h / n;

    rect.setHeight(ch);

    p->save();
    QColor c;
    foreach(c,colors)
    {
        p->fillRect(rect,c);
        rect.translate(QPoint(0,ch));
    }
    p->restore();
}

ColorExtractorTest::ColorExtractorTest(QStringList paths, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorExtractorTest),
    m_paths(paths),
    m_index(-1)
{
    ui->setupUi(this);
    on_next_clicked();
}

ColorExtractorTest::~ColorExtractorTest()
{
    delete ui;
}

void ColorExtractorTest::on_next_clicked()
{
    m_index++;
    int paths_size = m_paths.size();
    QString path = m_paths.at(m_index % paths_size);


    QList<ColorExtractrorOptions> options;
    for (int i=8;i<30;i=i+2)
    {
        int h = i*2;
        int s = i;
        int l = i;
        options << ColorExtractrorOptions(15,h,s,l);
    }

    QImage image(path);
    ColorExtractorSimple extractor(image);

    int w = 400;
    int h = 400;

    QImage palete(w,h,QImage::Format_RGB32);

    QPainter painter(&palete);

    int n = options.size();
    for (int i=0;i<n;i++)
    {
        ColorExtractrorOptions opt = options.at(i);
        QList<QColor> common = extractor.extract(opt);
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

    QImage neuquantPalette(w/10,h,QImage::Format_RGB32);

    ColorExtractorNeuquant neuquant(image);
    QList<QColor> common = neuquant.extract();
    QPainter neuquantPainter(&neuquantPalette);
    drawPalette(common,QRect(QPoint(0,0),neuquantPalette.size()),&neuquantPainter);

    ui->image->setPixmap(QPixmap::fromImage(image.scaled(QSize(w,h),Qt::KeepAspectRatio)));
    ui->colors->setPixmap(QPixmap::fromImage(palete));
    ui->neuquant->setPixmap(QPixmap::fromImage(neuquantPalette));
}
