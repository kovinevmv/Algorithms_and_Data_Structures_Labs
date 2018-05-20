#include "qcompressor.h"
#include <QString>

// Функция сжатия данных
bool QCompressor::gzipCompress(QByteArray input, QByteArray &output, int level)
{
    // Очистка массива
    output.clear();

    // Проверка на пустоту входной строки
    if(input.length())
    {

        int flush = 0;

        // Переменные статуса deflater
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        // Инициализация deflater
        int ret = deflateInit2(&strm, qMax(-1, qMin(9, level)), Z_DEFLATED, GZIP_WINDOWS_BIT, 8, Z_DEFAULT_STRATEGY);

        if (ret != Z_OK)
            return(false);


        output.clear();

        char *input_data = input.data();
        int input_data_left = input.length();

        // Сжатие данных, пока возможно
        do {

            // Определим размер chunck
            int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

            strm.next_in = (unsigned char*)input_data;
            strm.avail_in = chunk_size;

            input_data += chunk_size;
            input_data_left -= chunk_size;

            // Проверка на последний chunck
            flush = (input_data_left <= 0 ? Z_FINISH : Z_NO_FLUSH);

            // Сжатие chunck
            do {

                char out[GZIP_CHUNK_SIZE];
                strm.next_out = (unsigned char*)out;
                strm.avail_out = GZIP_CHUNK_SIZE;
                ret = deflate(&strm, flush);

                // Проверка ошибок
                if(ret == Z_STREAM_ERROR)
                {
                    // Очистка
                    deflateEnd(&strm);
                    return(false);
                }

                // Определим размер сжатого chunck
                int have = (GZIP_CHUNK_SIZE - strm.avail_out);

                if(have > 0)
                    output.append((char*)out, have);

            } while (strm.avail_out == 0);

        } while (flush != Z_FINISH);

        (void)deflateEnd(&strm);
        return(ret == Z_STREAM_END);
    }
    else
        return(true);
}


// Функция декомпрессии данных
bool QCompressor::gzipDecompress(QByteArray input, QByteArray &output)
{
    // Очистка массива
    output.clear();

    // Проверка на пустоту входной строки
    if(input.length() > 0)
    {
        // Переменные статуса deflater
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        // Инициализация inflater
        int ret = inflateInit2(&strm, GZIP_WINDOWS_BIT);

        if (ret != Z_OK)
            return(false);

        char *input_data = input.data();
        int input_data_left = input.length();

        // Декомпрессия, пока возможно
        do {

             // Определим размер chunck
            int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

             // Проверка на последний chunck
            if(chunk_size <= 0)
                break;

            strm.next_in = (unsigned char*)input_data;
            strm.avail_in = chunk_size;

            input_data += chunk_size;
            input_data_left -= chunk_size;

             // Декомпрессия chunck
            do {

                char out[GZIP_CHUNK_SIZE];

                strm.next_out = (unsigned char*)out;
                strm.avail_out = GZIP_CHUNK_SIZE;

                ret = inflate(&strm, Z_NO_FLUSH);

                // Проверка ошибок
                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                    // Очистка
                    inflateEnd(&strm);
                    return(false);
                }

                // Определим размер chunck
                int have = (GZIP_CHUNK_SIZE - strm.avail_out);

                if(have > 0)
                    output.append((char*)out, have);

            } while (strm.avail_out == 0);

        } while (ret != Z_STREAM_END);

        inflateEnd(&strm);
        return (ret == Z_STREAM_END);
    }
    else
        return(true);
}

// Функция шифрования данных XOR
QByteArray QCompressor::cryptData(QString a)
{
    QString x;
    QString key = ";";
    for (int i=0; i < a.size(); i++)
    {
        char aa = a.toStdString().c_str()[i];
        char kk = QString(key).toStdString().c_str()[i % key.size()];
        x[i] = aa ^ kk;
    }

    QByteArray compressed;
    QCompressor::gzipCompress(x.toLatin1(), compressed);

    return compressed;
}

// Функция дешифрования данных XOR
QString QCompressor::decryptData(QByteArray temp)
{

   QByteArray decompressed;
   QCompressor::gzipDecompress(temp, decompressed);
   QString x = QString::fromLatin1(decompressed);


    QString key = ";";
    for (int i=0; i < x.size(); i++)
    {
        char aa = x.toStdString().c_str()[i];
        char kk = QString(key).toStdString().c_str()[i % key.size()];
        x[i] = aa ^ kk;
    }
   return x;
}
