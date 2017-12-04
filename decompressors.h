#ifndef PATTERNS_DECOMPRESSORS_H
#define PATTERNS_DECOMPRESSORS_H

class NoDecompressor{};

template<typename T>
class DecompressorForCompilyator {
public:
    static T decompress(T &t) {
        return t;
    }
};

class DecompressDoubleToFloat{
public:
    static float decompress(double data){
        return static_cast<float>(data);
    }

private:
    float data;
};

#endif //PATTERNS_DECOMPRESSORS_H
