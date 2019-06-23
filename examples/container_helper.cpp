// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/container_helper.hpp>
#include <iostream>
#include <numeric>
#include <ostream>

using namespace ext;
using namespace util;
using namespace std;

template<typename Container>
void print(const Container& c) {
    for (auto i = c.begin(); i != c.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
}

int main() {
    vector<int> v(7);

    iota(v.begin(), v.end(), 1);
    print(v);
    for_each(v.begin(), v.end(), [](int& n) { n *= 11; });
    print(v);

    cout << "--" << endl;

    list<int> l(v.begin(), v.end());
    set<int> s(v.begin(), v.end());

    erase(v, 44);
    print(v);
    erase_if(v, [](int n) { return n % 2 == 0; });
    print(v);

    cout << "--" << endl;

    erase(l, 33);
    print(l);
    erase_if(l, [](int n) { return n % 2 != 0; });
    print(l);

    cout << "--" << endl;

    erase(s, 22);
    print(s);
    erase_if(s, [](int n) { return n > 40 && n < 70; });
    print(s);
}
