#ifndef PATTERNS_DECOMPRESSORS_H
#define PATTERNS_DECOMPRESSORS_H

class NoDecompressor {
public:
    static NoDecompressor decompress(void* v) {
        return NoDecompressor();
    }
};

class DecompressDoubleToFloat {
public:
    static float decompress(double *data) {
        return static_cast<float>(*data);
    }

private:
    float data;
};

#endif //PATTERNS_DECOMPRESSORS_H
