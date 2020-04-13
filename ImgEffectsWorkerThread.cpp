#include "ImgEffectsWorkerThread.h"

#include <vector>

using namespace std;

ImgEffectsWorkerThread::ImgEffectsWorkerThread()
    :m_nOperation(SearchCorners),
      m_ptrInput(nullptr),
      m_ptrResult(nullptr)
{
}

ImgEffectsWorkerThread::~ImgEffectsWorkerThread()
{

}

const QImage *ImgEffectsWorkerThread::getResultImage() const
{
    assert(isFinished());

    return m_ptrResult.data();
}

void ImgEffectsWorkerThread::startCornerSearch(const QImage &rcImage)
{
    if (isRunning())
        return;

    m_nOperation = SearchCorners;

    m_ptrInput.reset(new QImage(rcImage));

    QThread::start();
}

void ImgEffectsWorkerThread::stop()
{
    requestInterruption();
}

void ImgEffectsWorkerThread::run()
{
    sleep(10);
    switch (m_nOperation)
    {
    case SearchCorners:
        mSearchCorners();
        break;
    }
    if (isInterruptionRequested())
    {
        m_ptrResult.reset();
        return;
    }
}

void ImgEffectsWorkerThread::mSearchCorners()
{
    if (m_ptrInput)
    {
        m_ptrResult.reset(new QImage(*m_ptrInput));

        Mat image, src_gray, cornerMap;

        image = ImgConQTOpenCV::QImageToMat(*m_ptrInput.data(),false);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        cvtColor(image, src_gray, COLOR_BGR2GRAY);
        preCornerDetect(src_gray, cornerMap,7, BORDER_DEFAULT );

        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;

        minMaxLoc( cornerMap, &minVal, &maxVal, &minLoc, &maxLoc);

        threshold(cornerMap, cornerMap, maxVal * 0.001, 1.0 ,THRESH_BINARY);

        cornerMap.convertTo(cornerMap, CV_8U, 255);

        findContours(cornerMap, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

        vector<vector<Point>> contours_poly(contours.size());
        vector<Point2f> center(contours.size());
        vector<float> radius(contours.size());

        for( int i = 0; i < contours.size(); i++ )
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
        }

        float max_radius = *max_element(radius.begin(),radius.end());
        float min_radius = *min_element(radius.begin(),radius.end());
        float threshold_radius = (min_radius+max_radius)*0.25;

        for( int i = 0; i< contours.size(); i++ )
        {
            if(threshold_radius < radius[i])
            {
                Scalar color = Scalar(0,255,0);
                circle(image, center[i], (int)radius[i]+4, color, 2, 8, 0 );
            }
        }

        *m_ptrResult = ImgConQTOpenCV::MatToQImage(image);
    }
}
