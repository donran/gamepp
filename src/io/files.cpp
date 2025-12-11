#include "files.hpp"
#include <fstream>
#include <sstream>

namespace enginepp::io::files {

std::string read_file(const char *file_path) {
    std::ifstream ifs(file_path);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}
} // namespace enginepp::io::files
