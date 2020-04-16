#include "ImgEffectsManager.h"

#include "ImgEffectsWorkerThread.h"
#include <QThread>

ImgEffectsManager::ImgEffectsManager(QObject *parent) : QObject(parent)
{

}

void ImgEffectsManager::addEffectImg(const QImage image)
{
    m_ImageInput = image;
    ImgEffectsWorkerThread* effect=new ImgEffectsWorkerThread(image);
    QThread *thread = new QThread;
    effect->moveToThread(thread);
    connect(effect, SIGNAL(resultImage(QImage)), this, SLOT(onImgResult(QImage)));
    connect(thread, SIGNAL(started()), effect, SLOT(doWork()));
    connect(effect, SIGNAL(finished()), thread, SLOT(quit()));
    connect(effect, SIGNAL(finished()), effect, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

QImage ImgEffectsManager::getResultImage()
{
    return m_ImageResult;
}

void ImgEffectsManager::onImgResult(QImage image)
{
    m_ImageResult=image;
    emit refreshImage();
}
