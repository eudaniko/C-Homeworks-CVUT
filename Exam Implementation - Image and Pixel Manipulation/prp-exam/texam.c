#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "exam_utils.h"
#include "xwin_sdl.h"
#include "save_jpeg.h"

#define ERROR_101 101
#define ERROR_102 102
#define Default_Pallete_Size 256

typedef struct {
    int id;
    int R;
    int G;
    int B;

} RGBColor;

bool process_commands(FILE *input_commads, unsigned char *input_image, int width, int height, bool animation_is_active,
                      RGBColor *Pallete, int pallete_size);

bool copy_pixels(unsigned char *input_image, int width, int height, int x1, int y1, int delta_width, int delta_height,
                 int x2, int y2);

bool swap_pixels(unsigned char *input_image, int width, int height, int x1, int y1, int delta_width, int delta_height,
                 int x2, int y2);

bool set_color(int id, int R, int G, int B, RGBColor *Pallete, int pallete_size);

int find_color(int color_id, RGBColor *Pallete, int pallete_size);

bool print_color(unsigned char *input_image, int width, int height, int id, int x1, int y1, int delta_width,
                 int delta_height, RGBColor *Pallete, int pallete_size, bool animation_is_active);

bool mirror_image(unsigned char *input_image, int width, int height, bool animation_is_active);

bool mirror_image_vertical(unsigned char *input_image, int width, int height, bool animation_is_active);

int main(int argc, char *argv[]) {
    int _return = EXIT_SUCCESS;
    int pallete_size = Default_Pallete_Size;
    RGBColor *Pallete = malloc(pallete_size * sizeof(RGBColor));
    my_assert(Pallete != NULL, __func__, __LINE__, __FILE__);
    const char *input_image_name = argc > 1 ? argv[1] : NULL;
    const char *input_commands_name = argc > 2 ? argv[2] : NULL;
    const char *output_image_name = argc > 3 ? argv[3] : NULL;
    const bool animation_is_active = argc > 4 && !strcmp(argv[4], "--anim");
    my_assert(input_image_name != NULL && input_commands_name != NULL && output_image_name != NULL, __func__, __LINE__,
              __FILE__);
    int width, height;

    unsigned char *input_image = xwin_load_image(input_image_name, &width, &height);
    my_assert(input_image != NULL, __func__, __LINE__, __FILE__);
    if (animation_is_active)
        xwin_init(width, height);

    FILE *input_commands = fopen(input_commands_name, "r");

    if (input_commands == NULL) {
        fprintf(stderr, "ERROR: cannot open the file!\n");
        _return = ERROR_101;
    }

    if (!_return &&
        !process_commands(input_commands, input_image, width, height, animation_is_active, Pallete, pallete_size)) {
        fprintf(stderr, "ERROR: process the file!\n");
        _return = ERROR_102;
    }

    fclose(input_commands);

    if (!_return && (strstr(output_image_name, ".jpg") || strstr(output_image_name, ".jepg")))
        save_image_jpeg(width, height, input_image, output_image_name);
    else
        save_image_ppm(width, height, input_image, output_image_name);
    free(input_image);
    free(Pallete);
    return _return;
}

bool process_commands(FILE *input_commads, unsigned char *input_image, int width, int height, bool animation_is_active,
                      RGBColor *Pallete, int pallete_size) {
    bool _return = true;
    int p1, p2, delta_width, delta_height, p3, p4;
    char command[11];
    while (!feof(input_commads)) {
        int params = fscanf(input_commads, "%10s %d %d %d %d %d %d\n", command, &p1, &p2, &delta_width, &delta_height,
                            &p3, &p4);
        if (params == 7 && strcmp(command, "cp") == 0)
            _return = copy_pixels(input_image, width, height, p1, p2, delta_width, delta_height, p3, p4);
        else if (params == 7 && strcmp(command, "swap") == 0)
            _return = swap_pixels(input_image, width, height, p1, p2, delta_width, delta_height, p3, p4);
        else if (params == 5 && strcmp(command, "setcolor") == 0)
            _return = set_color(p1, p2, delta_width, delta_height, Pallete, pallete_size);
        else if (params == 6 && strcmp(command, "printcolor") == 0)
            _return = print_color(input_image, width, delta_height, p1, p2, delta_width, delta_height, p3, Pallete,
                                  pallete_size, animation_is_active);
        else if (params == 1 && strcmp(command, "mirror") == 0)
            _return = mirror_image(input_image, width, height, animation_is_active);
        else if (params == 1 && strcmp(command, "mirrorvert") == 0)
            _return = mirror_image_vertical(input_image, width, height, animation_is_active);

        else
            return false;

        if (animation_is_active) {
            xwin_redraw(width, height, input_image);
            delay(2000);
        }
    }
    return _return;
}

bool copy_pixels(unsigned char *input_image, int width, int height, int x1, int y1, int delta_width, int delta_height,
                 int x2, int y2) {
    bool _return = true;
    for (int xi = 0; xi < delta_width; xi++) {
        for (int yi = 0; yi < delta_height; yi++) {
            int sx = x1 + xi;
            int sy = y1 + yi;
            int dy = y2 + yi;
            int dx = x2 + xi;

            if (sx >= 0 && sx < width && sy >= 0 && sy < height &&
                dx >= 0 && dx < width && dy >= 0 && dy < height)
                for (int i = 0; i < 3; ++i) {
                    input_image[(dy * width + dx) * 3 + i] = input_image[(sy * width + sx) * 3 + i];
                }
            else
                return false;
        }
    }
    return _return;
}

bool swap_pixels(unsigned char *input_image, int width, int height, int x1, int y1, int delta_width, int delta_height,
                 int x2, int y2) {
    bool _return = true;
    for (int xi = 0; xi < delta_width; xi++) {
        for (int yi = 0; yi < delta_height; yi++) {
            int sx = x1 + xi;
            int sy = y1 + yi;
            int dy = y2 + yi;
            int dx = x2 + xi;

            if (sx >= 0 && sx < width && sy >= 0 && sy < height &&
                dx >= 0 && dx < width && dy >= 0 && dy < height)
                for (int i = 0; i < 3; ++i) {
                    unsigned char temp = input_image[(sy * width + sx) * 3 + i];
                    input_image[(sy * width + sx) * 3 + i] = input_image[(dy * width + dx) * 3 + i];
                    input_image[(dy * width + dx) * 3 + i] = temp;
                }
            else
                return false;
        }
    }
    return _return;
}

bool set_color(int id, int R, int G, int B, RGBColor *Pallete, int pallete_size) {
    static int color_index = 0;
    if (R < 0 || R > 255 || G < 0 || G > 255 || B < 0 || B > 255) {
        fprintf(stderr, "ERROR: uncknown color!\n");
        return false;
    }
    if (pallete_size > color_index) {
        Pallete[color_index].id = id;
        Pallete[color_index].R = R;
        Pallete[color_index].G = G;
        Pallete[color_index].B = B;
    } else if (pallete_size < color_index) {
        pallete_size *= 2;
        Pallete = realloc(Pallete, pallete_size * sizeof(RGBColor));
    } else {
        fprintf(stderr, "ERROR: cannot allocate memory for the new color!\n");
        return false;
    }
    color_index++;
    return true;
}

bool print_color(unsigned char *input_image, int width, int height, int id, int x1, int y1, int delta_width,
                 int delta_height, RGBColor *Pallete, int pallete_size, bool animation_is_active) {
    int color_index = find_color(id, Pallete, pallete_size);
    if (Pallete[color_index].id == id) {
        for (int xi = 0; xi < delta_width; xi++) {
            for (int yi = 0; yi < delta_height; yi++) {
                int sx = xi + x1;
                int sy = yi + y1;

                if (sx >= 0 && sx < width && sy >= 0 && sy < height) {
                    input_image[(sy * width + sx) * 3] = Pallete[color_index].R;
                    input_image[(sy * width + sx) * 3 + 1] = Pallete[color_index].G;
                    input_image[(sy * width + sx) * 3 + 2] = Pallete[color_index].B;
                } else {
                    fprintf(stderr, "ERROR: print color out of image size!\n");
                }
            }
        }
        if (animation_is_active)xwin_redraw(width, height, input_image);

    }

    return true;
}

int find_color(int color_id, RGBColor *Pallete, int pallete_size) {
    for (int index = 0; index < pallete_size; index++)
        if (Pallete[index].id == color_id)
            return index;
    return -1;
}

bool mirror_image(unsigned char *input_image, int width, int height, bool animation_is_active) {
    bool _return = true;
    for (int xi = 0; xi < width / 2; xi++) {
        for (int yi = 0; yi < height; yi++) {
            int ax = xi;
            int bx = width - xi - 1;
            for (int i = 0; i < 3; ++i) {
                unsigned char temp = input_image[(yi * width + ax) * 3 + i];
                // input_image[(yi * width + ax) * 3 + i] = input_image[(yi * width + bx) * 3 + i];
                input_image[(yi * width + bx) * 3 + i] = temp;
            }
        }
        if (animation_is_active) {
            xwin_redraw(width, height, input_image);
            delay(1);
        }
    }
    return _return;
}

bool mirror_image_vertical(unsigned char *input_image, int width, int height, bool animation_is_active) {
    bool _return = true;
    for (int yi = 0; yi < height / 2; yi++) {
        for (int xi = 0; xi < width; xi++) {
            int ay = yi;
            int by = height - yi - 1;
            for (int i = 0; i < 3; ++i) {
                unsigned char temp = input_image[(ay * width + xi) * 3 + i];
                input_image[(ay * width + xi) * 3 + i] = input_image[(by * width + xi) * 3 + i];
                input_image[(by * width + xi) * 3 + i] = temp;
            }
        }
        if (animation_is_active) {
            xwin_redraw(width, height, input_image);
            delay(1);
        }
    }
    return _return;
}

