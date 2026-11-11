
# Rossignol

## Build

```bash
# Debug
meson setup --native-file debug.ini build_debug
cd build_debug
# Release
meson setup --native-file release.ini build_release
cd build_release

ninja
```

## API overview

Rossignol has a basic image type which is essentially a 2D array of pixels. There are several instantiations with different pixel types. The `rol::image` type is an alias to a `std::variant` of all supported instantiations.

There are two different APIs that enable library users to interact with images.

### Low-level API

The low-level API is a collection of algorithms defined in the `rol::algo` namespace. Most algorithms take a specific kind of image (not the `rol::image` variant) as an input and also output a specific image type. When possible, the parameters of these algorithms are passed as template parameters (usually via an enum).

```cpp
// Algorithms
#include <rossignol/algorithm/binarise.hh>
#include <rossignol/algorithm/greyscale.hh>

// For reading and writing files
#include <rossignol/image/io.hh>


int main() {
    const std::filesystem::path image_path("./birb.png");
    rol::parsed_image_t img_in = rol::load_image(image_path);

    // Images don't have a default constructor so we'll use a pointer here
    std::unique_ptr<rol::greyscale_image> img_grey = nullptr;
    if(std::holds_alternative<rol::greyscale_image>(img_in)) {
        // The image is already in greyscale format => no conversion to be performed
        // Sharing an image object allows to create a new image pointing at the same buffer, avoiding a useless copy
        img_grey = std::make_unique<rol::greyscale_image>(std::get<rol::greyscale_image>(img_in).share());
    } else if(std::holds_alternative<rol::rgb_image>(img_in)) {
        // Convert RGB to greyscale and store in the pointer
        img_grey = std::make_unique<rol::greyscale_image>(rol::algo::greyscale_luma(std::get<rol::rgb_image>(img_in)));
    } else {
        // No other image types can actually be returned by `rol::parse_image`
        throw std::runtime_error("Unsupported image type");
    }

    // Now that we have a greyscale image, let's apply the Floyd-Steinberg dithering algorithm
    rol::binary_image img_dither = rol::algo::floyd_steinberg(*img_grey);

    // The result is a binary image (only black or white pixels, encoded on booleans) but this type is not directly writable to a file
    // Hence, we have to convert it back to greyscale
    rol::greyscale_image img_out = rol::algo::greyscale(img_dither);

    rol::write_image(img_out, out_path);

    return 0;
}
```

### High-level API

The high-level API encapsulates the low-level APIs in functor objects that can receive either the generic `rol::image` type or some specific types. They return the most specific possible type, or throw an exception when the input type is not supported. Operator overloading allows "piping" images between these operators. Because the specific image types cannot be easily converted (by copy) to the generic variant, it is possible to prevent some unsupported operations at compile time.

```cpp
// Generic operators
#include <rossignol/generic/binarise.hh>
#include <rossignol/generic/greyscale.hh>

// This enables "piping" images between operators
#include <rossignol/generic/pipeline.hh>

// For reading and writing files
#include <rossignol/image/io.hh>
#include <rossignol/generic/sinks/file.hh>


int main() {
    const std::filesystem::path image_path("./birb.png");
    rol::rgb_image img_in = rol::load_image_rgba(image_path);

    // Note: the parameter of `rol::generic::to_greyscale` is a variant, `rol::generic::luma` is a struct that contains a parameter for the luma coefficients
    rol::generic::image img_out = img_in | rol::generic::to_greyscale(rol::generic::luma()) | rol::generic::binarise(rol::generic::floyd_steinberg) | rol::generic::to_greyscale();

    img_out > rol::generic::file_sink("./out.png");

    return 0;
}
```

## Acknowledgements

- Colourmaps come from GNU Octave, MATLAB, Matplotlib and Google
- Initial implementation of colourmaps was inspired by OpenCV
