#include <iostream>

#include "utils/tgaimage.h"
#include "utils/model.h"
#include "utils/geometry.h"

const TGAColor white = {
        .bgra[0] = 255,
        .bgra[1] = 255,
        .bgra[2] = 255,
        .bgra[3] = 255
};
const TGAColor red = {
        .bgra[0] = 0,
        .bgra[1] = 0,
        .bgra[2] = 255,
        .bgra[3] = 255
};
Model *model = NULL;
const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

int main(int argc, char **argv) {
//    if (2==argc) {
//        model = new Model(argv[1]);
//    } else {
    model = new Model("../../src/utils/obj/african_head/african_head.obj");
//    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces() * 3; i += 3) {
        for (int j = 0; j < 3; ++j) {
            vec<3> v0 = model->vert(model->get_tmp_facet_vrt()[i + j]);
            vec<3> v1 = model->vert(model->get_tmp_facet_vrt()[i + (j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            line(x0, y0, x1, y1, image, white);
        }
    }

//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
}
