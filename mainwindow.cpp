#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ImageLabel(new QLabel),
      m_ScrollArea(new QScrollArea),
      m_Manager(new ImgEffectsManager(this))
{
    m_ScrollArea->setBackgroundRole(QPalette::Dark);
    m_ScrollArea->setWidget(m_ImageLabel);
    m_ScrollArea->setVisible(false);
    setCentralWidget(m_ScrollArea);

    resize(QGuiApplication::primaryScreen()->availableSize() * 1/2);

    createActions();

    connect(m_Manager,SIGNAL(refreshImage()),SLOT(onRefreshImage()));
}

MainWindow::~MainWindow()
{

}

bool MainWindow::loadFile(const QString & fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(fileName)).arg(m_Image.width()).arg(m_Image.height()).arg(m_Image.depth());
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
            ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Image Processing"),
                       tr("Find all angles in the image using the function"
                          "preCornerDetect(), pre-color the image to shades of gray."
                          "Select all found angles in small circles in the original image."));
}

void MainWindow::searchCorners()
{
    m_Manager->addEffectImg(m_Image);

    m_EditMenu->setEnabled(false);
    m_SaveAsAct->setEnabled(false);
    m_OpenAct->setEnabled(false);
}

void MainWindow::onRefreshImage()
{
    m_Image=m_Manager->getResultImage();
    m_ImageLabel->setPixmap(QPixmap::fromImage(m_Image));

    m_EditMenu->setEnabled(true);
    m_SaveAsAct->setEnabled(true);
    m_OpenAct->setEnabled(true);
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    m_OpenAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    m_OpenAct->setShortcut(QKeySequence::Open);

    m_SaveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    m_SaveAsAct->setEnabled(false);

    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    m_EditMenu = menuBar()->addMenu(tr("&Edit"));

    m_SearchCorners = m_EditMenu->addAction(tr("&Search Сorners"), this, &MainWindow::searchCorners);
    m_SearchCorners->setShortcut(tr("Ctrl+R"));
    m_SearchCorners->setEnabled(false);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

void MainWindow::setImage(const QImage &newImage)
{
    m_Image = newImage;
    if (m_Image.colorSpace().isValid())
        m_Image.convertToColorSpace(QColorSpace::SRgb);
    m_ImageLabel->setPixmap(QPixmap::fromImage(m_Image));

    m_ScrollArea->setVisible(true);
    updateActions();

    m_ImageLabel->adjustSize();
}

void MainWindow::updateActions()
{
    m_SaveAsAct->setEnabled(!m_Image.isNull());
    m_SearchCorners->setEnabled(!m_Image.isNull());
}

bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(m_Image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

