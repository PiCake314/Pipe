#include <iostream>
#include <concepts>

template <typename T>
concept container = requires(T t){
    t.begin();
    t.end();
    typename T::value_type;
};



template <typename INIT, typename OP>
struct Fold{
    INIT init;
    OP func;

    template<container V>
    auto operator()(const V &v)
    requires std::invocable<OP, INIT, typename V::value_type>
    {
        for(const auto &elt : v)
            init = func(init, elt);

        return init;
    }
};


template <container V, typename INIT, typename OP>
requires std::invocable<OP, INIT, typename V::value_type>
INIT operator | (V v, Fold<INIT, OP> folder){
    return folder(v);
}

template <typename INIT, typename F>
Fold<INIT, F> fold(INIT init, F func){
    return {init, func};
}

template <container V, typename INIT, typename F>
INIT fold(V v, INIT init, F func){
    return Fold<INIT, F>{init, func}(v);
}


int main(){
    using std::operator""s;

    std::vector v = {3, 14, 1, 5, 9, 2, 6, 5, 3, 5};


    auto res_call = fold(v, 0, [](int acc, int elt) -> int { return acc + elt; }); // this works

    auto res_pipe = v | fold(""s, [](std::string acc, int elt) -> std::string { return acc + std::to_string(elt); }); // this works

    std::cout << res_call << '\n';
    std::cout << res_pipe << '\n';

    return 0;
}