#ifndef IMGCONVERTERQTVSOPENCV_H
#define IMGCONVERTERQTVSOPENCV_H

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGlobal>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"


namespace ImgConQTOpenCV {
using namespace cv;
static QMap<QImage::Format, QString>   sImageFormatEnumMap = {
   { QImage::Format::Format_Invalid, "Format_Invalid" },
   { QImage::Format::Format_Mono, "Format_Mono" },
   { QImage::Format::Format_MonoLSB, "Format_MonoLSB" },
   { QImage::Format::Format_Indexed8, "Format_Indexed8" },
   { QImage::Format::Format_RGB32, "Format_RGB32" },
   { QImage::Format::Format_ARGB32, "Format_ARGB32" },
   { QImage::Format::Format_ARGB32_Premultiplied, "Format_ARGB32_Premultiplied" },
   { QImage::Format::Format_RGB16, "Format_RGB16" },
   { QImage::Format::Format_ARGB8565_Premultiplied, "Format_ARGB8565_Premultiplied" },
   { QImage::Format::Format_RGB666, "Format_RGB666" },
   { QImage::Format::Format_ARGB6666_Premultiplied, "Format_ARGB6666_Premultiplied" },
   { QImage::Format::Format_RGB555, "Format_RGB555" },
   { QImage::Format::Format_ARGB8555_Premultiplied, "Format_ARGB8555_Premultiplied" },
   { QImage::Format::Format_RGB888, "Format_RGB888" },
   { QImage::Format::Format_RGB444, "Format_RGB444" },
   { QImage::Format::Format_ARGB4444_Premultiplied, "Format_ARGB4444_Premultiplied" },
   { QImage::Format::Format_RGBX8888, "Format_RGBX8888" },
   { QImage::Format::Format_RGBA8888, "Format_RGBA8888" },
   { QImage::Format::Format_RGBA8888_Premultiplied, "Format_RGBA8888_Premultiplied" },
   { QImage::Format::Format_BGR30, "Format_BGR30" },
   { QImage::Format::Format_A2BGR30_Premultiplied, "Format_A2BGR30_Premultiplied" },
   { QImage::Format::Format_RGB30, "Format_RGB30" },
   { QImage::Format::Format_A2RGB30_Premultiplied, "Format_A2RGB30_Premultiplied" },
   { QImage::Format::Format_Alpha8, "Format_Alpha8" },
   { QImage::Format::Format_Grayscale8, "Format_Grayscale8" }
};

static QString sQImageFormatToStr( QImage::Format inFormat )
{
   return sImageFormatEnumMap[inFormat];
}

static QString sCVTypeToStr( int inType )
{
   QString str( "CV_" );

   switch ( CV_MAT_DEPTH( inType ) )
   {
      case CV_8U:    str += "8U"; break;
      case CV_8S:    str += "8S"; break;
      case CV_16U:   str += "16U"; break;
      case CV_16S:   str += "16S"; break;
      case CV_32S:   str += "32S"; break;
      case CV_32F:   str += "32F"; break;
      case CV_64F:   str += "64F"; break;
      default:       str += "User"; break;
   }

   str += QStringLiteral( "C%1" ).arg( QString::number( CV_MAT_CN( inType ) ) );

   return str;
}

inline QImage  MatToQImage( const Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "ImgConQTOpenCV::MatToQImage() - Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

inline QPixmap MatToQPixmap( const Mat &inMat )
{
    return QPixmap::fromImage( MatToQImage( inMat ) );
}

inline Mat QImageToMat( const QImage &inImage, bool inCloneImageData = true )
{
    switch ( inImage.format() )
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        Mat  mat( inImage.height(), inImage.width(),
                  CV_8UC4,
                  const_cast<uchar*>(inImage.bits()),
                  static_cast<size_t>(inImage.bytesPerLine())
                  );

        return (inCloneImageData ? mat.clone() : mat);
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ImgConQTOpenCV::QImageToMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        Mat  mat( inImage.height(), inImage.width(),
                  CV_8UC4,
                  const_cast<uchar*>(inImage.bits()),
                  static_cast<size_t>(inImage.bytesPerLine())
                  );

        Mat  matNoAlpha;

        cvtColor( mat, matNoAlpha, COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ImgConQTOpenCV::QImageToMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        QImage   swapped = inImage.rgbSwapped();

        return Mat( swapped.height(), swapped.width(),
                    CV_8UC3,
                    const_cast<uchar*>(swapped.bits()),
                    static_cast<size_t>(swapped.bytesPerLine())
                    ).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        Mat  mat( inImage.height(), inImage.width(),
                  CV_8UC1,
                  const_cast<uchar*>(inImage.bits()),
                  static_cast<size_t>(inImage.bytesPerLine())
                  );

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qWarning() << "ImgConQTOpenCV::QImageToMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return Mat();
}

inline Mat QPixmapToMat( const QPixmap &inPixmap, bool inCloneImageData = true )
{
    return QImageToMat( inPixmap.toImage(), inCloneImageData );
}
}


#endif // IMGCONVERTERQTVSOPENCV_H
