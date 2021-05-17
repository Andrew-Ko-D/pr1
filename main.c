#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* A coloured pixel. */

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} pixel_t;

/* A picture. */

typedef struct {
    pixel_t* pixels;
    size_t width;
    size_t height;
} bitmap_t;

/* Given "bitmap", this returns the pixel of bitmap at the point
("x", "y"). */

static pixel_t* pixel_at(bitmap_t* bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
success, non-zero on error. */

static int save_png_to_file(bitmap_t* bitmap, const char* path)
{
    FILE* fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte** row_pointers = NULL;
    /* "status" contains the return value of this function. At first
    it is set to a value which means 'failure'. When the routine
    has finished its work, it is set to a value which means
    'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
    see where it it is documented in the libpng manual.
    */
    int pixel_size = 4;
    int depth = 8;

    fp = fopen(path, "wb");
    if (!fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    /* Set up error handling. */

    if (setjmp(png_jmpbuf(png_ptr))) {
        goto png_failure;
    }

    /* Set image attributes. */

    png_set_IHDR(png_ptr,
        info_ptr,
        bitmap->width,
        bitmap->height,
        depth,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc(png_ptr, bitmap->height * sizeof(png_byte*));
    for (y = 0; y < bitmap->height; ++y) {
        png_byte* row =
            png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; ++x) {
            pixel_t* pixel = pixel_at(bitmap, x, y);
            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
            *row++ = pixel->alpha;
        }
    }

    /* Write the image data to "fp". */

    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
    "status" to a value which indicates success. */

    status = 0;

    for (y = 0; y < bitmap->height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
    png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
    fclose(fp);
fopen_failed:
    return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
to take, this returns an integer between 0 and 255 proportional to
"value" divided by "max". */

static int pix(int value, int max)
{
    if (value < 0)
        return 0;
    return (int)(256.0 * ((double)(value) / (double)max));
}

int main(int argc, char **argv[])
{
    bitmap_t fruit;
    int x;
    int y;
    int l = 4;

    /* Create an image. */

    fruit.width = 10 * l;
    fruit.height = 10 * l;
    fruit.pixels = calloc(sizeof(pixel_t), fruit.width * fruit.height);

    for (y = 0; y < fruit.height; y++)
    {
        for (x = 0; x < fruit.width; x++)
        {
            pixel_t* pixel = pixel_at(&fruit, x, y);
            pixel->red = 255;
            pixel->green = 255;
            pixel->blue = 255;
            pixel->alpha = 255;
        }
    }
    int m = fruit.height/l;
    int n = fruit.width/l;
    int* a = (int*)malloc(n * sizeof(int));;
    srand(time(NULL));
    a[0] = rand() % n;
    for (int i = 1; i < n; i++)
    {
    m:
        a[i] = 1 + rand() % m;
        for (int j = 0; j < i; j++)
        {
            if (a[i] == a[j])
                goto m;
        }
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d", a[i]);
    }
    /*for (x = 0; x < n; x++)
    {
        pixel_t* pixel = pixel_at(&fruit, x, m - a[x]);
        pixel->red = 0;
        pixel->green = 0;
        pixel->blue = 0;

        for (y = m - a[x] + 1; y < m; y++)
        {
            pixel_t* pixel = pixel_at(&fruit, x, y);
            pixel->red = 100;
            pixel->green = 100;
            pixel->blue = 100;
        }

    }*/

    for (x = 0; x < fruit.width; x = x + l)
    {
        for (int w = 0; w < l; w++)
        {
            for (int h = 0; h < l; h++)
            {
                pixel_t* pixel = pixel_at(&fruit, x + w, fruit.height - (a[x/l]*l) + h);
                pixel->red = 0;
                pixel->green = 0;
                pixel->blue = 0;

                for (y = (fruit.height - a[x/l]*l) + l; y < fruit.height; y = y + l)
                {
                    for (int w1 = 0; w1 < l; w1++)
                    {
                        for (int h1 = 0; h1 < l; h1++)
                        {
                            pixel_t* pixel = pixel_at(&fruit, x + w1, y + h1);
                            pixel->red = 100;
                            pixel->green = 100;
                            pixel->blue = 100;
                        }
                    }
                }
            }
        }
    }

    char str[100];

    int count = 1;
    sprintf(str, "frame%05d.png", count);
    count++;
    save_png_to_file(&fruit, str);

    /*for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (a[j] < a[i])
            {
                int* p = &a[i];
                int* l = &a[j];
                int z = *p;
                *p = *l;
                *l = z;

                pixel_t* pixel = pixel_at(&fruit, i, m - a[j]);
                pixel->red = 255;
                pixel->green = 255;
                pixel->blue = 255;

                pixel = pixel_at(&fruit, i, m - a[i]);
                pixel->red = 0;
                pixel->green = 0;
                pixel->blue = 0;

                pixel = pixel_at(&fruit, j, m - a[i]);
                pixel->red = 255;
                pixel->green = 255;
                pixel->blue = 255;

                pixel = pixel_at(&fruit, j, m - a[j]);
                pixel->red = 0;
                pixel->green = 0;
                pixel->blue = 0;

                for (int k = m - a[i] - 1; k > m - a[j]; k--)
                {
                    pixel = pixel_at(&fruit, i, k);
                    pixel->red = 255;
                    pixel->green = 255;
                    pixel->blue = 255;
                }

                for (int c = m - a[j] + 1; c < m - a[i] + 1; c++)
                {
                    pixel = pixel_at(&fruit, j, c);
                    pixel->red = 100;
                    pixel->green = 100;
                    pixel->blue = 100;

                }

                sprintf(str, "frame%05d.png", count);
                save_png_to_file(&fruit, str);
                count++;
            }
        }
    }*/


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                int* p = &a[j];
                int* b = &a[j + 1];
                int z = *p;
                *p = *b;
                *b = z;

                /*int z = a[j];
                a[j] = a[j + 1];
                a[j + 1] = z;*/
                for (int w = 0; w < l; w++)
                {
                    for (int h = 0; h < l; h++)
                    {
                        pixel_t* pixel = pixel_at(&fruit, j*l + w, fruit.height - a[j + 1] * l + h);
                        pixel->red = 255;
                        pixel->green = 255;
                        pixel->blue = 255;
                    }
                }
                for (int w = 0; w < l; w++)
                {
                    for (int h = 0; h < l; h++)
                    {
                        pixel_t* pixel = pixel_at(&fruit, j*l + w, fruit.height - a[j] * l + h);
                        pixel->red = 0;
                        pixel->green = 0;
                        pixel->blue = 0;
                    }
                }
                   
                for (int w = 0; w < l; w++)
                {
                    for (int h = 0; h < l; h++)
                    {
                        pixel_t* pixel = pixel_at(&fruit, (j + 1) * l + w, fruit.height - a[j] * l + h);
                        pixel->red = 255;
                        pixel->green = 255;
                        pixel->blue = 255;
                    }
                }
                for (int w = 0; w < l; w++)
                {
                    for (int h = 0; h < l; h++)
                    {
                        pixel_t* pixel = pixel_at(&fruit, (j + 1) * l + w, fruit.height - a[j + 1] * l + h);
                        pixel->red = 0;
                        pixel->green = 0;
                        pixel->blue = 0;
                    }
                }

               for (int k = fruit.height - a[j] * l - l; k > fruit.height - a[j + 1] * l; k = k - l)
                {
                   for (int w = 0; w < l; w++)
                   {
                       for (int h = 0; h < l; h++)
                       {
                           pixel_t* pixel = pixel_at(&fruit, j * l + w, k + h);
                           pixel->red = 255;
                           pixel->green = 255;
                           pixel->blue = 255;
                       }
                   }
                }

                for (int c = fruit.height - a[j + 1] * l + l; c < fruit.height - a[j] * l + l; c = c + l)
                {
                    for (int w = 0; w < l; w++)
                    {
                        for (int h = 0; h < l; h++)
                        {
                            pixel_t* pixel = pixel_at(&fruit, (j + 1) * l + w, c + h);
                            pixel->red = 100;
                            pixel->green = 100;
                            pixel->blue = 100;
                        }
                    }
                }

                sprintf(str, "frame%05d.png", count);
                save_png_to_file(&fruit, str);
                count++;
            }
        }
    }

    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d", a[i]);
    }
    sprintf(str, "frame%05d.png", count);
    save_png_to_file(&fruit, str);

    free(a);

    free(fruit.pixels);

    /*for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            free(&fruit, i, j);
        }
    }

    free(&fruit);*/

    /*pixel = pixel_at(&fruit, 10, 5);
    pixel->red = 0;
    pixel->green = 0;
    pixel->blue = 0;

    pixel = pixel_at(&fruit, 1, 1);
    pixel->red = 0;
    pixel->green = 0;
    pixel->blue = 0;*/



    /* Write the image to a file 'fruit.png'. */

    return 0;
}
