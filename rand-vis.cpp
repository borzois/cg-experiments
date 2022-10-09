#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include <iostream>
#include <vector>

/* Visualizer for C rand()
 * Usage: ./rand_vis [w] [h] [iterations] > img.ppm
 * Use a ppm viewer to open the image
 */

struct vec3 {
    int r;
    int g;
    int b;
};

struct canvas_size {
    int w;
    int h;
};

void draw_bg(std::vector<std::vector<vec3>> &canvas, const canvas_size csize) {
    int image_width = csize.w; int image_height = csize.h;

    for (int i = image_height - 1; i >= 0; i--) {
        // generates the gradient (pretty sure i stole this part from like raytracing in one weekend)
        auto r = 1 - double(i) / (image_height - 1);
        auto g = 1 - double(i) / (image_height - 1);
        auto b = 1;

        // converts the color values from 0.0-1.0 floats to 0-255 ints
        int ir = static_cast<int>(r * 255.999);
        int ig = static_cast<int>(g * 255.999);
        int ib = static_cast<int>(b * 255.999);

        for (int j = 0; j < image_width; j++) {
            canvas[i][j].r = ir;
            canvas[i][j].g = ig;
            canvas[i][j].b = ib;
        }
    }
    std::cerr << "BG generated\n";
}

void draw_rand(std::vector<std::vector<vec3>> &canvas, const canvas_size csize, const int iterations) {
    int image_width = csize.w; int image_height = csize.h;
    int range = 200;

    int origin_x = image_height / 8;
    int origin_y = (image_width - range) / 2;

    srand(time(nullptr));
    for (int k = 0; k < iterations; k++) {
        int random_number = rand() % range;

        int pos_x = origin_x;
        int pos_y = origin_y + random_number;
        while(canvas[pos_x][pos_y].b == 0 && pos_x <= image_height) {
            pos_x++;
        }

        canvas[pos_x][pos_y].r = 0;
        canvas[pos_x][pos_y].g = 0;
        canvas[pos_x][pos_y].b = 0;
    }
}

void render(const std::vector<std::vector<vec3>> &canvas, const canvas_size csize) {
    int image_width = csize.w; int image_height = csize.h;

    // writes the header
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // writes the pixel data
    for (int i = image_height - 1; i >= 0; i--) {
        for (int j = 0; j < image_width; j++) {
            std::cout << canvas[i][j].r << ' ' << canvas[i][j].g << ' ' << canvas[i][j].b << '\n';
        }
    }
    std::cerr << "Render finished\n";
}

canvas_size parse_size(int argc, char** argv) {
    if (argc >= 3) {
        return canvas_size {
                static_cast<int>(strtol(argv[1], nullptr, 10)),
                static_cast<int>(strtol(argv[2], nullptr, 10))
        };
    }
    return canvas_size { 256, 256 }; // default
}

int parse_iterations(int argc, char** argv) {
    if (argc >= 4) {
        return static_cast<int>(strtol(argv[3], nullptr, 10));
    }
    return 200;
}

int main(int argc, char** argv)
{
    // gets the size of the image
    canvas_size csize = parse_size(argc, argv);
    std::cerr << "W: " << csize.w << " H: " << csize.h << '\n';

    // initializes a 2d vector made of rgb values
    std::vector<std::vector<vec3>> canvas(csize.h, std::vector<vec3>(csize.w));

    // prepares canvas
    draw_bg(canvas, csize);

    int rand_iterations = parse_iterations(argc, argv);
    draw_rand(canvas, csize, rand_iterations);

    // writes to file
    render(canvas, csize);

    return 0;
}