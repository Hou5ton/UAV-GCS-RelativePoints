#ifndef FILEVALIDATOR_H
#define FILEVALIDATOR_H

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

class  FileValidator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool fileValid READ isFileValid NOTIFY fileValidChanged)
    Q_PROPERTY(QString fileErrorMessage READ fileErrorMessage WRITE setFileErrorMessage NOTIFY fileErrorMessageChanged)
    Q_PROPERTY(bool treatAsImage READ treatAsImage WRITE setTreatAsImage NOTIFY treatAsImageChanged)

public:
    explicit FileValidator(QObject *parent = nullptr);

    QUrl url() const;
    void setUrl(const QUrl &url);

    bool isFileValid() const;

    QString fileErrorMessage() const;
    void setFileErrorMessage(const QString &fileErrorMessage);

    bool treatAsImage() const;
    void setTreatAsImage(bool treatAsImage);

signals:
    void urlChanged();
    void fileValidChanged();
    void fileErrorMessageChanged();
    void treatAsImageChanged();

protected:
    virtual void validate();

    QUrl mUrl;
    QString mFileErrorMessage;
    bool mTreatAsImage;
};

#endif // FILEVALIDATOR_H
