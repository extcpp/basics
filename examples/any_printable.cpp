#include <obi/util/any_printable.hpp>
using namespace obi::util;

//function from other tu - note: no template required
void take_any(any_printable const& any);

// this could be in another tu
void take_any(any_printable const& any){
	std::cout << any << std::endl;
}

//custom type
struct developer{
    std::string data;
};

std::ostream& operator <<(std::ostream& os, developer const& dev){
    return os << dev.data;
}

int main(/*int argc, const char *argv[]*/) {
    take_any(42);
    take_any("ulf");
    take_any(355.0/113.0);
    take_any(developer{"gondel"});
}
