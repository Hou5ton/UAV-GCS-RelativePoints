#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QBuffer>
#include <QImageWriter>

class ImageSaver : public QObject
{
    Q_OBJECT
public:
    ImageSaver() : QObject() {
        moveToThread(&t);
        t.start();
    }
    ~ImageSaver() {
       // qDebug("!!!");
        t.quit();
        t.wait();
    }

    /**
    * Request a new image to be saved.
    * @param image the image to save.
    * @param fileName file name of image.
    * @param absPath the absolute path of where to save the image.
    * @param lat latitude of screenshot.
    * @param lon longitude of screenshot.
    * @param alt altitude of screenshot.
    */
    void requestImageSave(QImage image,
                          QString fileName,
                          QString absPath,
                          QString absPathPrev,
                          double lat,
                          double lon,
                          int alt) {

        QMetaObject::invokeMethod(this,
                                  "saveImage",
                                  Q_ARG(QImage, image),
                                  Q_ARG(QString, fileName),
                                  Q_ARG(QString, absPath),
                                  Q_ARG(QString, absPathPrev),
                                  Q_ARG(double, lat),
                                  Q_ARG(double, lon),
                                  Q_ARG(int, alt));
    }

    public slots:
    /**
    * Needs to be a slot to be invokable with QMetaObject.
    * Performs the saving of the image.
    * @param image the image to save.
    * @param fileName file name of image.
    * @param absPath the absolute path of where to save the image.
    * @param lat latitude of screenshot.
    * @param lon longitude of screenshot.
    * @param alt altitude of screenshot.
    */
    void saveImage(QImage image,
                   QString fileName,
                   QString absPath,
                   QString absPathPrev,
                   double lat,
                   double lon,
                   int alt) {
        bool saved = false;
        QByteArray qbaFile;
        QByteArray qbaFilePrev;
        QString qsError = "";

        if(lat == 0.0){
          qsError = tr("Invalid latitude");
        }else if (lon == 0.0) {
          qsError = tr("Invalid longitude");
        } else if (alt == 0) {
          qsError = tr("Invalid altitude");
        }else{
            saved = image.save(absPath);

            if(saved){
                QBuffer buffer;
                QImageWriter writer(&buffer,"PNG");
                writer.write(image);
                qbaFile = buffer.data();

                QImage small = image.scaled(150,150);
                saved = small.save(absPathPrev);
                // QImage small = image.scaled(50,50);
                //saved = small.save(absPathPrev,nullptr,50);

                if(saved){
                    QBuffer buffer;
                    QImageWriter writer(&buffer,"PNG");
                    writer.write(small);
                    qbaFilePrev = buffer.data();
                }
            }
        }

        emit imageReady(fileName, qbaFile,qbaFilePrev,lat,lon,alt,saved,qsError);
    }

signals:
    /**
    * Signal to connect to after the image has been saved.
    */
    void imageReady(QString fileName,
                    QByteArray qbaFile,
                    QByteArray qbaFilePrev,
                    double lat,
                    double lon,
                    int alt,
                    bool state,
                    QString error);

private:
    /**
    * Worker thread
    */
    QThread t;
};

#endif // IMAGESAVER_H
