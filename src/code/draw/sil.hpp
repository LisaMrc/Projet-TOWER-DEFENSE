#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

namespace sil {

class Image {
public:
    /// Loads an image. The path can either be absolute or relative (in which case it will be relative to the directory containing your CMakeLists.txt file).
    explicit Image(std::filesystem::path const& path);
    /// Creates a black image with the given size.
    Image(int width, int height);

    int width() const { return _width; }
    int height() const { return _height; }
    /// Returns the color of the pixel (expressed in sRGB space).
    /// x = 0 corresponds to the left of the image.
    /// y = 0 corresponds to the bottom of the image.
    glm::vec3& pixel(int x, int y);
    /// Returns the color of the pixel (expressed in sRGB space).
    /// x = 0 corresponds to the left of the image.
    /// y = 0 corresponds to the bottom of the image.
    glm::vec3 const& pixel(int x, int y) const;

    /// Returns the entire list of pixels. They are stored contiguously, row after row, from left to right and from bottom to top.
    std::vector<glm::vec3>& pixels() { return _pixels; }
    /// Returns the entire list of pixels. They are stored contiguously, row after row, from left to right and from bottom to top.
    std::vector<glm::vec3> const& pixels() const { return _pixels; };

    /// Saves the image as either jpeg or png based on the extension you put in the `path`.
    /// Example: `image.save("final_image.png");`
    void save(std::filesystem::path path);

private:
    // Color in sRGB.
    // Stored row by row, from left to right and from bottom to top.
    std::vector<glm::vec3> _pixels;
    int                    _width;
    int                    _height;
};

} // namespace sil