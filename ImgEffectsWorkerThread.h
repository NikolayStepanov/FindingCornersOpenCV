#ifndef IMGEFFECTSWORKERTHREAD_H
#define IMGEFFECTSWORKERTHREAD_H

#include <ImgConverterQTvsOpenCV.h>
#include <QThread>

using namespace ImgConQTOpenCV;

class ImgEffectsWorkerThread: public QThread
{
    Q_OBJECT
public:
    explicit ImgEffectsWorkerThread();
    ~ImgEffectsWorkerThread();

public:
    const QImage *getResultImage() const;
    void startCornerSearch(const QImage &rcImage);
    void stop();

private:
    virtual void run();
    void mSearchCorners();

private:
    enum Operation
    {
        SearchCorners
    };

    Operation m_nOperation;
    QScopedPointer <QImage> m_ptrInput, m_ptrResult;

};

#endif // IMGEFFECTSWORKERTHREAD_H
