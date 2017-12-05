#ifndef PATTERNS_COMPRESSEDTYPES_H
#define PATTERNS_COMPRESSEDTYPES_H

#include <istream>

class NoneType {
public:
    NoneType decompress() const {
        return NoneType();
    }
};

class MyChar {
public:
    MyChar(char c) : data(c) {}

    MyChar() {};

protected:
    friend std::istream &operator >>(std::istream &is, MyChar &obj);
    char data;
};

std::istream &operator >> (std::istream &is, MyChar &obj) {
    is >> obj.data;
    return is;
}

class MyIntFromChar : public MyChar {
public:
    int decompress() const {
        return static_cast<int>(data);
    }

private:
};

#endif //PATTERNS_COMPRESSEDTYPES_H
