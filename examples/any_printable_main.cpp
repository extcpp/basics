// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/any_printable.hpp>
using namespace ext::util;

//function from other tu - note: no template required
void use(any_printable const& any);

//custom type
struct developer{
    std::string data;
};

std::ostream& operator <<(std::ostream& os, developer const& dev){
    return os << dev.data;
}

int main(/*int argc, const char *argv[]*/) {
    use(42);
    use("ulf");
    use(355.0/113.0);
    use(developer{"gondel"});
}
