#include <iostream>
#include <fstream>
#include "compressedTypes.h"
#include "decompressors.h"

struct EmptyNode {
};

template<typename ...Args>
struct TypeList {
    typedef EmptyNode _head;
    typedef EmptyNode _tail;
};

typedef TypeList<> EmptyTypeList;

template<typename Head, typename ...Tail>
struct TypeList<Head, Tail...> {
    typedef Head _head;
    typedef TypeList<Tail...> _tail;
};

//--------------------------------------
template<typename TL>
struct GetSize {
};

template<>
struct GetSize<EmptyNode> {
    static const int size = 0;
};

template<typename ...Args>
struct GetSize<TypeList<Args...>> {
    static const int size =
            sizeof(typename TypeList<Args...>::_head) + GetSize<typename TypeList<Args...>::_tail>::size;
};

template<typename TL1,typename TL2,typename TL3, int shift>
struct InnerReader {
    static void read(std::istream &is, void *ptr) {};
};

template<typename Head1, typename ...Tail1, typename Head2, typename ...Tail2, typename Head3, typename ...Tail3, int shift>
struct InnerReader<TypeList<Head1, Tail1...>,TypeList<Head2, Tail2...>,TypeList<Head3, Tail3...>, shift> {
    static void read(std::istream &is, void *ptr) {
        if (!std::is_same<Head1, EmptyNode>::value) {
            Head1 compressedObj;
            is >> compressedObj;
            if (std::is_same<Head2, NoneType>::value){
                using Head3_ = DecompressorForCompilyator<Head3>;
                auto obj = Head3_::decompress(compressedObj);
                typeof(obj) *objPtr = static_cast<typeof(obj) *>(ptr + shift);
                *objPtr = obj;
                InnerReader<TypeList<Tail1...>,TypeList<Tail2...>,TypeList<Tail3...>, shift + sizeof(obj)>::read(is, ptr);
            } else{
                Head2 *castObj = reinterpret_cast<Head2*>(&compressedObj);
                auto obj = (*castObj).decompress();
                typeof(obj) *objPtr = static_cast<typeof(obj) *>(ptr + shift);
                InnerReader<TypeList<Tail1...>,TypeList<Tail2...>,TypeList<Tail3...>, shift + sizeof(obj)>::read(is, ptr);
            }

        }
    }
};

template<typename TL, std::size_t shift>
struct InnerPrint {

    static void print(std::ostream &os, const void *ptr) {}
};

template<typename Head, typename ...Tail, std::size_t shift>
struct InnerPrint<TypeList<Head, Tail...>, shift> {

    static void print(std::ostream &os, const void *ptr) {
        if (!std::is_same<Head, EmptyNode>::value) {
            const Head *objPtr = static_cast<const Head *>(ptr + shift);
            os << *objPtr << " ";
            InnerPrint<TypeList<Tail...>, shift + sizeof(Head)>::print(os, ptr);
        }
    }
};

template<typename TL1, typename TL2, typename TL3>
class Reader {
public:
    Reader(std::string fileName = "../input.txt") {
        is.open(fileName, std::ifstream::in);
    }

    void *nextLine() {
        static const int typelist1Size = GetSize<TL1>::size;
        void *ptr = malloc(typelist1Size);
        InnerReader<TL1, TL2, TL3, 0>::read(is, ptr);
        return ptr;
    }

    void print(std::ostream &os, const void *ptr) {
        InnerPrint<TL1, 0>::print(os, ptr);
    };
private:
    std::ifstream is;
};

template<typename TL>
class Printer {
public:

    static void print(std::ostream &os, const void *ptr) {
        InnerPrint<TL, 0>::print(os, ptr);
    };
private:
    std::ifstream is;
};


int main() {

    //using TypeList1 = TypeList<int, int, double, char>;
    using TypeList1 = TypeList<double, char, double>;
    using TypeList2 = TypeList<NoneType, int, float>;
    using TypeList3 = TypeList<DecompressDoubleToFloat, NoDecompressor, NoDecompressor>;
    using PrintTypeList = TypeList<float, int, float>;

    Reader<TypeList1, TypeList2, TypeList3> reader;

    for (int i = 0; i < 1; i++) {
        void *ptr = reader.nextLine();
        Printer<PrintTypeList>::print(std::cout, ptr);
        std::cout << std::endl;
    }

    return 0;
};