#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <vector>
#include <string>

class File {
public:
    File();
    ~File();
    bool open(std::string filename);
    bool create(std::string filename);

    void seek(long p);
    off_t pos();
    bool atEnd();
    unsigned long length() { return size; }

    int readInt();
    int readInt64();
    void readChar(char *dest, int n);
    std::vector<unsigned char> read(long n);

    int writeInt(int n);
    int writeInt64(int n);
    int writeChar(char *source, int n);
    int write(std::vector<unsigned char> &v);

protected:
    unsigned long size;
    FILE *file;
};

#endif // FILE_H
