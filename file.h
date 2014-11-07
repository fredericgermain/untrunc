#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <vector>
#include <string>

class FileContent {
public:
    FileContent() : _size(0), _data(NULL) {}
    FileContent(long size, unsigned char *data) : _size(size), _data(data) {}

    long size() { return _size; }
    unsigned char *begin() { return _data; }
    unsigned char &operator[](long idx) { return _data[idx];}

    void resize(long new_size) { _size = new_size; }
    void swap(FileContent &b) { 
      long __size;
      unsigned char *__data;
      __size = b._size;
      b._size = _size;
      _size = __size;
      __data = b._data;
      b._data = _data;
      _data = __data;
   }
protected:
    long _size;
    unsigned char *_data;
};

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
    FileContent *read(long n);

    int writeInt(int n);
    int writeInt64(int n);
    int writeChar(char *source, int n);
    int write(FileContent&);

protected:
    unsigned long size;
    FILE *file;
    unsigned char *mmap_data;
};

#endif // FILE_H
