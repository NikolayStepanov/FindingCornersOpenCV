#ifndef IMGEFFECTSMANAGER_H
#define IMGEFFECTSMANAGER_H

#include <QObject>
#include <QImage>

class ImgEffectsManager : public QObject
{
    Q_OBJECT
public:
    explicit ImgEffectsManager(QObject *parent = nullptr);

    void addEffectImg(const QImage image);
    QImage getResultImage();
signals:
    void refreshImage();

public slots:
    void onImgResult(QImage image);

private:
    QImage m_ImageResult;
    QImage m_ImageInput;
};

#endif // IMGEFFECTSMANAGER_H
