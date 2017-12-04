#ifndef PATTERNS_COMPRESSEDTYPES_H
#define PATTERNS_COMPRESSEDTYPES_H

class NoneType {
public:
    NoneType decompress() const {
        return NoneType();
    }
};
#endif //PATTERNS_COMPRESSEDTYPES_H
