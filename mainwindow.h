#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include <ImgEffectsWorkerThread.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool loadFile(const QString & fileName);

protected:
    virtual void closeEvent(QCloseEvent *pEvent);

private slots:
    void open();
    void saveAs();
    void about();
    void searchCorners();

    void onThreadStarted();
    void onThreadFinished();

private:
    void createActions();
    void setImage(const QImage &newImage);
    void updateActions();
    bool saveFile(const QString &fileName);


private:
    QImage m_Image;

    QLabel* m_ImageLabel;
    QScrollArea* m_ScrollArea;
    QAction* m_SaveAsAct;
    QAction* m_OpenAct;
    QAction* m_SearchCorners;
    QMenu* m_EditMenu;

    ImgEffectsWorkerThread * m_pThreadWork;
};
#endif // MAINWINDOW_H
