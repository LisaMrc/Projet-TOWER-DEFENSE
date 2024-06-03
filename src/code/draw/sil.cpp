#include "sil.hpp"
#include <algorithm>
#include <img/img.hpp>
#include <iostream>

namespace sil {

static std::filesystem::path make_absolute_path(std::filesystem::path const& path, bool check_path_exists)
{
    auto const res = [&]() { // IIFE
        if (!path.is_relative())
            return path;
        return SIL_CMAKE_SOURCE_DIR / path;
    }();

    if (check_path_exists && !std::filesystem::exists(res))
    {
        using namespace std::literals;
        auto const msg = "Path \""s + res.string() + "\" does not exist. Make sure you didn't misspell it or made it relative to the wrong folder. All relative paths will be relative to the directory containing your CMakeLists.txt file.";
        std::cerr << msg << '\n';
        throw std::runtime_error{msg};
    }

    return res;
}

static void make_directories_if_necessary(std::filesystem::path path)
{
    path = path.parent_path(); // Remove the filename part, keep only the directory path
    if (!std::filesystem::is_directory(path)
        || !std::filesystem::exists(path))
    {
        std::filesystem::create_directories(path);
    }
}

Image::Image(int width, int height)
    : _pixels(static_cast<size_t>(width) * static_cast<size_t>(height))
    , _width{width}
    , _height{height}
{
}

Image::Image(std::filesystem::path const& path)
{
    auto const image = img::load(make_absolute_path(path, true /*check_path_exists*/), 3);
    _width           = static_cast<int>(image.width());
    _height          = static_cast<int>(image.height());
    _pixels.resize(static_cast<size_t>(_width) * static_cast<size_t>(_height));
    for (size_t i = 0; i < _pixels.size(); ++i)
    {
        _pixels[i].r = static_cast<float>(image.data()[3 * i + 0]) / 255.f; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        _pixels[i].g = static_cast<float>(image.data()[3 * i + 1]) / 255.f; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        _pixels[i].b = static_cast<float>(image.data()[3 * i + 2]) / 255.f; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
}

void Image::save(std::filesystem::path path)
{
    auto const extension = path.extension();
    bool const is_png    = extension == ".png";
    bool const is_jpeg   = extension == ".jpeg"
                         || extension == ".jpg";
    if (!is_png && !is_jpeg)
    {
        using namespace std::literals;
        auto const msg = "Extension \""s + extension.string() + "\" is not supported. Use .png or .jpeg";
        std::cerr << msg << '\n';
        throw std::runtime_error{msg};
    }

    auto* data = new uint8_t[static_cast<size_t>(_width) * static_cast<size_t>(_height) * 3]; // NOLINT(cppcoreguidelines-owning-memory)
    for (size_t i = 0; i < _pixels.size(); ++i)
    {
        data[3 * i + 0] = static_cast<uint8_t>(std::clamp(std::floor(_pixels[i].r * 256.f), 0.f, 255.f)); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        data[3 * i + 1] = static_cast<uint8_t>(std::clamp(std::floor(_pixels[i].g * 256.f), 0.f, 255.f)); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        data[3 * i + 2] = static_cast<uint8_t>(std::clamp(std::floor(_pixels[i].b * 256.f), 0.f, 255.f)); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    auto const image = img::Image{{static_cast<unsigned int>(_width), static_cast<unsigned int>(_height)}, 3, data};

    path = make_absolute_path(path, false /*check_path_exists*/);
    make_directories_if_necessary(path);
    if (is_png)
        img::save_png(path, image);
    else
        img::save_jpeg(path, image);
}

glm::vec3& Image::pixel(int x, int y)
{
    assert(x >= 0);
    assert(x < _width);
    assert(y >= 0);
    assert(y < _height);
    return _pixels[x + y * _width];
}

glm::vec3 const& Image::pixel(int x, int y) const
{
    assert(x >= 0);
    assert(x < _width);
    assert(y >= 0);
    assert(y < _height);
    return _pixels[x + y * _width];
}

} // namespace sil