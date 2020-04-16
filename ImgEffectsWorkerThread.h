#ifndef IMGEFFECTSWORKERTHREAD_H
#define IMGEFFECTSWORKERTHREAD_H

#include <QObject>

class ImgEffectsWorkerThread: public QObject
{
    Q_OBJECT
public:
    explicit ImgEffectsWorkerThread(const QImage image);
    ~ImgEffectsWorkerThread();

signals:
    void finished();
    void resultImage(QImage m_ptrResul);

public slots:
    void doWork();
    void stopWork();

private:
    QScopedPointer <QImage> m_ptrInput, m_ptrResult;

};

#endif // IMGEFFECTSWORKERTHREAD_H
