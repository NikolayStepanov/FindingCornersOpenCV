#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

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

private slots:
    void open();
    void saveAs();
    void about();

private:
    void createActions();
    void setImage(const QImage &newImage);
    void updateActions();
    bool saveFile(const QString &fileName);


private:
    QImage image;

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QAction *saveAsAct;
};
#endif // MAINWINDOW_H
