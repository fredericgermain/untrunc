#include "file.h"
#include <string>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

#define PAGESIZE 4*1024
//#define USE_MMAP

static void reverse(int &input) {
    int output;
    char *a = ( char* )&input;
    char *b = ( char* )&output;

    b[0] = a[3];
    b[1] = a[2];
    b[2] = a[1];
    b[3] = a[0];
    input = output;
}

File::File(): file(NULL) {
}

File::~File() {
    if(file) {
#ifdef USE_MMAP
        munmap(mmap_data, ((size+PAGESIZE-1)/PAGESIZE)*PAGESIZE);
#endif
        fclose(file);
    }
}

bool File::open(string filename) {
    file = fopen(filename.c_str(), "r");
    if(file == NULL) return false;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

#ifdef USE_MMAP
    mmap_data = (unsigned char*)mmap(NULL, ((size+PAGESIZE-1)/PAGESIZE)*PAGESIZE, PROT_READ, 0, fileno(file), 0);
    if (mmap_data == (void*)-1)
       fprintf(stderr, "errno:%d\n", errno);
#endif

    return true;
}

bool File::create(string filename) {
    file = fopen(filename.c_str(), "wb");
    if(file == NULL) return false;
    return true;
}

void File::seek(off_t p) {
    fseek(file, p, SEEK_SET);
}

off_t File::pos() {
    return ftell(file);
}

bool File::atEnd() {
    off_t pos = ftell(file);
    return pos == size;
}

int File::readInt() {
    int value;
    int n = fread(&value, sizeof(int), 1, file);
    if(n != 1)
        throw string("Could not read atom length");
    reverse(value);
    return value;
}

int File::readInt64() {
    int hi, low;
    int n = fread(&hi, sizeof(int), 1, file);
    if(n != 1)
        throw string("Could not read atom length");
    n = fread(&low, sizeof(int), 1, file);
    if(n != 1)
        throw string("Could not read atom length");

    reverse(low);
    return low;
}

void File::readChar(char *dest, int n) {
    int len = fread(dest, sizeof(char), n, file);
    if(len != n)
        throw string("Could not read chars");
}

FileContent *File::read(off_t n) {
    off_t _pos = pos();
    FileContent *ret;
    //vector<unsigned char> dest(n);
    //off_t len = fread(&*dest.begin(), sizeof(unsigned char), n, file);
    if(_pos + n > size)
        throw string("Could not read at position");
#ifdef USE_MMAP
    ret = new FileContent(n, &mmap_data[_pos]);
    seek(_pos + n);
#else
    unsigned char *data = (unsigned char *)malloc(n);
    ret = new FileContent(n, data);
    while (n != 0) {
       size_t block_size;
       if (n >= 1*1024*1024)
           block_size = 1*1024*1024;
       else
           block_size = n;
       fread(data, sizeof(unsigned char), block_size, file);
       data += block_size;
       n -= block_size;
    }
#endif
    return ret;
}

int File::writeInt(int n) {
    reverse(n);
    fwrite(&n, sizeof(int), 1, file);
    return 4;
}

int File::writeInt64(int n) {
    int hi = 0;
    reverse(n);
    fwrite(&hi, sizeof(int), 1, file);
    fwrite(&n, sizeof(int), 1, file);
    return 8;
}

int File::writeChar(char *source, int n) {
    fwrite(source, 1, n, file);
    return n;
}

int File::write(FileContent& v) {
    fwrite(&*v.begin(), 1, v.size(), file);
    return v.size();
}
