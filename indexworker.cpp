#include "indexworker.h"

#include <QImageReader>
#include <QDir>
#include <QImage>
#include <QCryptographicHash>
#include <QBuffer>

#include "databasesettings.h"
#include "colorextractorsimple.h"

IndexWorker::IndexWorker(QObject *parent) :
    QObject(parent)
{

}

void IndexWorker::scanDirectories(const QStringList& dirs, const QStringList &toRemove) {

    Q_UNUSED(toRemove)

    QStringList imgFilter;
    foreach (QByteArray format, QImageReader::supportedImageFormats())
        if (format.toLower() == format)
            imgFilter.append("*." + format);

    // scan directories for subdirectories
    QString t_dir;
    QStringList subdirs;
    QStringList dirFilter;
    dirFilter << "*";
    /*int i = 0;
    int n = dirs.size();*/

    foreach(t_dir,dirs) {
        QDir dir(t_dir);
        subdirs << t_dir;
        QStringList t_subdirs = dir.entryList(dirFilter,QDir::Dirs);
        QString subdir;
        foreach(subdir,t_subdirs) {
            if (subdir != "." && subdir != "..")
                subdirs.append( dir.absolutePath() + QDir::separator() + subdir);
        }
        //emit progress(++i*1000/n);
    }
    //emit progress(1000);

    // scan directories for files
    int chunkSize = 100;
    QStringList filesChunk;
    foreach(t_dir,subdirs) {
        QDir dir(t_dir);
        QStringList t_files = dir.entryList(imgFilter,QDir::Files);
        while (t_files.size()>0) {
            filesChunk.append(t_files.takeFirst());
            if (filesChunk.size()==chunkSize) {
                emit filesScaned(filesChunk);
                filesChunk.clear();
            }
        }
    }

    if (filesChunk.size()>0) {
        emit filesScaned(filesChunk);
        filesChunk.clear();
    }

}

void IndexWorker::filesUnindexed(const QStringList& files) {

    QString file;
    int i = 0;
    int n = files.size();

    int chunkSize = 100;
    QList<ImageStatistics> filesChunk;

    foreach (file,files) {

        QImage image(file);
        ColorExtractorSimple extractor(image);
        QList<QColor> common = extractor.extract();

        QImage preview = extractor.scaled().scaled(QSize(150,150),Qt::KeepAspectRatio);

        QByteArray previewByteArray;

        QBuffer buf(&previewByteArray);
        buf.open(QIODevice::WriteOnly);
        preview.save(&buf,"JPEG");

        QString md5 = QString(QCryptographicHash::hash(previewByteArray,QCryptographicHash::Md5).toHex());
        QString previewFileName = md5 + QString(".jpg");

        QFile previewFile(QDir(m_previewDir).filePath(previewFileName));
        previewFile.open(QIODevice::WriteOnly);
        previewFile.write(previewByteArray);
        previewFile.close();

        filesChunk.append(ImageStatistics(file,previewFileName,common));

        if (filesChunk.size() == chunkSize) {
            emit filesAnalyzed(filesChunk);
            filesChunk.clear();
        }
        emit progress(++i*1000/n);
    }

    if (filesChunk.size()>0) {
        emit filesAnalyzed(filesChunk);
        filesChunk.clear();
    }
    emit progress(1000);
}

void IndexWorker::openDatabase(const QStringList& settings) {
    m_previewDir = settings.at(DatabaseSettings::PreviewDir);
}
