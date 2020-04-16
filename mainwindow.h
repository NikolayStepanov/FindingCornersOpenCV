#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include "ImgEffectsManager.h"

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
    void onRefreshImage();

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

    ImgEffectsManager * m_Manager;
};
#endif // MAINWINDOW_H
