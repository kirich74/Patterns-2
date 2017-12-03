#include <iostream>
#include <fstream>

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

template <typename TL, int shift>
struct InnerReader {
    static void read(std::istream &is, void *ptr) {};
};

template <typename Head, typename ...Tail, int shift>
struct InnerReader<TypeList<Head, Tail...>, shift> {
    static void read(std::istream &is, void *ptr) {
        if (!std::is_same<Head, EmptyNode>::value) {
            Head obj;
            is >> obj;
            Head *objPtr = static_cast<Head *>(ptr + shift);
            *objPtr = obj;
            InnerReader<TypeList<Tail...>, shift + sizeof(Head)>::read(is, ptr);
        }
    }
};

template <typename TL, std::size_t shift>
struct InnerPrint {

    static void print(std::ostream &os, const void *ptr) {}
};

template <typename Head, typename ...Tail, std::size_t shift>
struct InnerPrint<TypeList<Head, Tail...>, shift> {

    static void print(std::ostream &os, const void *ptr) {
        if (!std::is_same<Head, EmptyNode>::value) {
            const Head *objPtr = static_cast<const Head *>(ptr + shift);
            os << *objPtr << " ";
            InnerPrint<TypeList<Tail...>, shift + sizeof(Head)>::print(os, ptr);
        }
    }
};

template <typename TL>
class Reader {
public:
    static void* nextLine(std::istream& is) {
        static const int typelistSize = GetSize<TL>::size;
        void* ptr = malloc(typelistSize);
        InnerReader<TL, 0>::read(is, ptr);
        return ptr;
    }
    static void print(std::ostream &os, const void *ptr) {
        InnerPrint<TL, 0>::print(os, ptr);
    };
};



int main() {

    using GivenTypeList = TypeList<int, int, double, char>;

    std::ifstream is("../input.txt");

    void *ptr = Reader<GivenTypeList>::nextLine(is);

    Reader<GivenTypeList>::print(std::cout, ptr);

    return 0;
};