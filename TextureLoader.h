#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <stb/stb_image.h>
#include <vector>
#include "Vec3.h"

Vec3f& Normalize(Vec3f& p)
{
    Vec3f normalized;
    float mag = p.x * p.x + p.y * p.y + p.z * p.z;
    mag = pow(mag, 0.5);
    normalized = p.operator/(mag);
    return normalized;
}

bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y)
{
    int numColCh;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &numColCh, 4);
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}
//void GetPixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, stbi_uc* r, stbi_uc* g, stbi_uc* b) {
//    const stbi_uc* p = image + (4 * (y * imageWidth + x));
//    *r = p[0];
//    *g = p[1];
//    *b = p[2];
//}

int width, height;
Vec3f& GetPixel(int x, int y)
{
    std::string filename = "OBJFiles/brick.png";

    //int width, height;
    std::vector<unsigned char> image;
    bool success = load_image(image, filename, width, height);
    if (!success)
    {
        std::cout << "Error loading image\n";
    }

    const size_t RGBA = 4;

    size_t index = RGBA * (y * width + x);
    int r = static_cast<int>(image[index + 0]);
    int g = static_cast<int>(image[index + 1]);
    int b = static_cast<int>(image[index + 2]);
    int a = static_cast<int>(image[index + 3]);
    Vec3f pixelColor = Vec3f(float(r), float(g), float(b));
    Vec3f nomalizedColor = Normalize(pixelColor);
    return nomalizedColor;
}

int getWidth()
{
    return width;
}

int getHeight()
{
    return height;
}
