#pragma once
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;
namespace enginepp::io::assets {
static fs::path g_base_path("./assets/");

namespace images {
static fs::path images_base_path = g_base_path / "images";
static fs::path Path(const char *filename) {
    return images_base_path / filename;
}
static std::string ReadFile(const char *file_path) {
    std::ifstream ifs((images_base_path / file_path).c_str());
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}
}; // namespace images

namespace shaders {
static fs::path shaders_base_path = g_base_path / "shaders";
static fs::path Path(const char *filename) {
    return shaders_base_path / filename;
}
static std::string ReadFile(const char *file_path) {
    std::ifstream ifs((shaders_base_path / file_path).c_str());
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}
}; // namespace shaders

}; // namespace enginepp::io::assets
