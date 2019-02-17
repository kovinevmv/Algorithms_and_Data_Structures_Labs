#ifndef QCOMPRESSOR_H
#define QCOMPRESSOR_H

#include <zlib.h>
#include <QByteArray>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

class QCompressor
{
public:
    // Функция сжатия текста
    static bool gzipCompress(QByteArray input, QByteArray &output, int level = -1);
    // Функция декомпрессии текста
    static bool gzipDecompress(QByteArray input, QByteArray &output);
    static QByteArray cryptData(QString input);       // Функция шифрования тектса
    static QString  decryptData(QByteArray input);    // Функция дешифрования текста
};

#endif // QCOMPRESSOR_H
