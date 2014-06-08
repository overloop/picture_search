#include "imagedialog.h"
#include "ui_imagedialog.h"

#include "colorextractorneuquant.h"
#include "colorextractorsimple.h"
#include "colorsmodel.h"

#include "colorsview.h"

#include "NEUQUANT.H"

#include <QDebug>

ImageDialog::ImageDialog(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);

    //ui->image->setImage(path);

    QImage image(path);
    Q_ASSERT(image.format() == QImage::Format_RGB32);

    /*ColorExtractorNeuquant extractor(image);
    QList<QColor> colors = extractor.extract();

    ColorsModel* model = new ColorsModel(colors,this);
    ui->colors->setModel(model);*/

    QImage converted = image.convertToFormat(QImage::Format_RGB888);

    /*qDebug() << (converted.scanLine(1) - converted.scanLine(0));
    qDebug() << image.width()*3;
    qDebug() << (image.scanLine(1)-image.scanLine(0));*/

    int sf = 10;

    int w = image.width();
    int h = image.height();

    uchar* image_data;

    image_data = (uchar*) malloc(w*h*3);
    for (int i=0;i<h;i++)
        memcpy(image_data+i*w*3,converted.scanLine(i),w*3);

    initnet(image_data,3*w*h,sf);
    learn();
    unbiasnet();
    inxbuild();
    uchar* colormap = (uchar*)malloc(netsize*3);
    writecolormap(colormap);

    QList<QColor> colors;
    for (int i=0;i<netsize;i++)
        colors.append(QColor::fromRgb(colormap[i*3+2],colormap[i*3],colormap[i*3]));

    QImage* sampled = new QImage(image.size(),QImage::Format_RGB888);

    for (int j=0;j<h;j++) {
        for (int i=0;i<w;i++) {
            QRgb rgb = image.pixel(i,j);
            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);

            int c = inxsearch(b,g,r);

            *(sampled->scanLine(j)+i*3+0) = colormap[c*3+0];
            *(sampled->scanLine(j)+i*3+1) = colormap[c*3+1];
            *(sampled->scanLine(j)+i*3+2) = colormap[c*3+2];
        }
    }

    ui->image->setPixmap(QPixmap::fromImage(*sampled));

    free(image_data);
    free(colormap);



}

ImageDialog::~ImageDialog()
{
    delete ui;
}
