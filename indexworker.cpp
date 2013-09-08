#include "indexworker.h"

#include <QImageReader>
#include <QDir>
#include <QImage>
#include <QCryptographicHash>
#include <QBuffer>
#include <QDebug>

#include "databasesettings.h"
#include "colorextractorsimple.h"

IndexWorker::IndexWorker(QObject *parent) :
    QObject(parent), m_total(0)
{

}

void IndexWorker::scanDirectories(const QStringList& dirs, const QStringList &toRemove) {

    Q_UNUSED(toRemove)

    m_time.start();

    QStringList imgFilter;
    foreach (QByteArray format, QImageReader::supportedImageFormats())
        if (format.toLower() == format)
            imgFilter.append("*." + format);

    // scan directories for subdirectories
    QString t_dir;
    QString subdir;
    QStringList subdirs;
    QStringList dirFilter;
    dirFilter << "*";

    foreach(t_dir,dirs) {
        QDir dir(t_dir);
        subdirs << t_dir;
        QStringList t_subdirs = dir.entryList(dirFilter,QDir::Dirs);
        foreach(subdir,t_subdirs) {
            if (subdir != "." && subdir != "..")
                subdirs.append( dir.absolutePath() + QDir::separator() + subdir);
        }
    }

    // scan directories for files
    QStringList filesChunk;

    m_total = 0;
    m_done = 0;
    foreach(t_dir,subdirs) {
        QDir dir(t_dir);
        QStringList t_files = dir.entryList(imgFilter,QDir::Files);
        m_total += t_files.size();
    }

    foreach(t_dir,subdirs) {
        QDir dir(t_dir);
        QStringList t_files = dir.entryList(imgFilter,QDir::Files);
        QString file;
        foreach(file,t_files) {
            filesChunk.append(dir.filePath(file));
            if (filesChunk.size()==CHUNK_SIZE) {
                emit filesScaned(filesChunk);
                filesChunk.clear();
            }
        }
    }

    /* поправляем тотал на последний чанк, этот последний чанк будет всегда, даже если нет файлов
     * чтобы зарепортить окончание операции
     */
    m_total += CHUNK_SIZE;
    m_total -= filesChunk.size();
    emit filesScaned(filesChunk);
    filesChunk.clear();
}

void IndexWorker::filesUnindexed(const QStringList& files) {

    QString file;

    /* поправляем тотал, вычитая из него количество файлов которые уже есть в индексе,
     * эти файлы отсеялись между filesScaned(chunk1) и filesUnindexed(chunk2): chunk2.size()<chunk1.size()
     */
    m_total -= (CHUNK_SIZE - files.size());

    ImageStatisticsList filesChunk;

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

        filesChunk.append(ImageStatistics(-1,file,previewFileName,common));

        ++m_done;
        if (m_total>0)
            emit progress(qMin(qMax(m_done*1000/m_total,0),1000));
        //qDebug() << m_done << m_total;
    }

    if (m_done == m_total) {
        emit filesAnalyzed(filesChunk,true,m_time);
        filesChunk.clear();
        emit status("Writing data to database, please wait.");
    } else {
        emit filesAnalyzed(filesChunk,false,QTime());
        filesChunk.clear();
    }

}

void IndexWorker::openDatabase(const QStringList& settings) {
    m_previewDir = settings.at(DatabaseSettings::PreviewDir);
}
