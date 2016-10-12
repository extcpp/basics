#include <string>
#include <algorithm>
namespace obi { namespace util {
std::string basename(std::string const& pathname){
    return std::string(std::find_if(pathname.rbegin()
                                   ,pathname.rend()
                                   ,[](char c) { return c == '/' || c =='\\';}
                                   ).base()
                      ,pathname.end());
}
}} // obi::util
