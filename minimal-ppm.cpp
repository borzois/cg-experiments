#include <iostream>

/* Minimal ppm renderer
 * Draws an ugly ass gradient bg
 * Usage: ./minimal-img > out.ppm
 */

struct vec3 {
    int r;
    int g;
    int b;
};

void draw_bg(vec3 canvas[256][256], int image_height, int image_width) {
    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            auto r = 1 - double(j) / (image_height-1);
            auto g = 1 - double(j) / (image_height-1);
            auto b = 1;

            // converts the color values from 0.0-1.0 floats to 0-255 ints
            int ir = static_cast<int>(r * 255.999);
            int ig = static_cast<int>(g * 255.999);
            int ib = static_cast<int>(b * 255.999);

            canvas[i][j].r = ir;
            canvas[i][j].g = ig;
            canvas[i][j].b = ib;
        }
    }
}

void render(vec3 canvas[256][256], int image_height, int image_width) {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            std::cout << canvas[i][j].r << ' ' << canvas[i][j].g << ' ' << canvas[i][j].b << '\n';
        }
    }
}

int main(int argc, char** argv)
{
    const int image_width = 256;
    const int image_height = 256;
    vec3 canvas[image_width][image_height];

    // prepare canvas
    draw_bg(canvas, image_height, image_width);

    /* stuff goes here */

    // write to file
    render(canvas, image_height, image_width);

    return 0;
}