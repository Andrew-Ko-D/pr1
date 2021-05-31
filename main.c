#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

void fill_matrix_white(bitmap_t fruit, int x, int y, int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 255;
            pixel->green = 255;
            pixel->blue = 255;
        }
    }
}
void fill_matrix_red(bitmap_t fruit, int x, int y,int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 255;
            pixel->green = 0;
            pixel->blue = 0;
        }
    }
}
void fill_matrix_green(bitmap_t fruit, int x, int y, int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 0;
            pixel->green = 255;
            pixel->blue = 0;
        }
    }
}
void fill_matrix_blue(bitmap_t fruit, int x, int y, int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 0;
            pixel->green = 0;
            pixel->blue = 255;
        }
    }
}
void fill_matrix_black(bitmap_t fruit, int x, int y, int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 0;
            pixel->green = 0;
            pixel->blue = 0;
        }
    }
}
void fill_matrix_gray(bitmap_t fruit, int x, int y, int l)
{
    for (int w = 0; w < l; w++)
    {
        for (int h = 0; h < l; h++)
        {
            pixel_t* pixel = pixel_at(&fruit, x + w, y + h);
            pixel->red = 100;
            pixel->green = 100;
            pixel->blue = 100;
        }
    }
}
void fill_matrix0 (bitmap_t fruit, int x, int y, int m, int n)
{
    
    for (y = 0; y < m; y++)
    {
        for (x = 0; x < n; x++)
        {
            pixel_t* pixel = pixel_at(&fruit, x, y);
            pixel->red = 255;
            pixel->green = 255;
            pixel->blue = 255;
            pixel->alpha = 255;
        }
    }
}
void fill_array(int* a, int m, int n)
{
    a[0] = rand() % m + 1;
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
}
void print_array(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", a[i]);
    }
}
void swap(int* x, int* y)
{
    int z = *x;
    *x = *y;
    *y = z;
}

void check_help(int argc, char** argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-h", argv[i]) == 0)
        {
            printf("Program for bubble sort visualization.\nGeneral options :\n- n[--number]\tNumber of values to sort\n- s[--size]\tBox size for each value(in pixels)\n- o[--order]\tDemonstrate stability or not\n- c[--complexity]\tPrint number of swaps / comparisions\n- h[--help]\tShow help\n\n$ bubblesort.exe -n 25 -s 10 -o -c\nDone!\n");
            printf("Post-scriptu:if the user does not use the matrix or pixel size input function, the default values are used\n");
            printf("default values:\nnumber = 10\nsize = 3");
            exit(EXIT_SUCCESS);
        }
    }

}

void check_s(int argc, char** argv[], int l)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-s", argv[i]) == 0)
        {
            if (i == argc - 1)
            {
                printf("Error\nSpecify the size of the pixel!!!");
                exit(EXIT_SUCCESS);
            }
            int ss = atoi(argv[i + 1]);
            if (ss >= 1)
            {
                //swap(&l, &ss);
                l = ss;
            }
            else
            {
                printf("Error\n");
                printf("incorrect pixel size indication\n");
                printf("The pixel size must be greater than 1!!!\n");
                printf("Call the function -h\n");
                exit(EXIT_SUCCESS);
            }

        }
    }
}
void check_n(int argc, char** argv[], bitmap_t fruit)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-n", argv[i]) == 0)
        {
            if (i == argc - 1)
            {
                printf("Error\nSpecify the size of the matrix!!!");
                exit(EXIT_SUCCESS);
            }
            int sn = atoi(argv[i + 1]);
            if (sn >= 5)
            {
                fruit.width = sn;
                fruit.height = sn;
            }
            else
            {
                printf("Error\n");
                printf("incorrect matrix size indication\n");
                printf("The matrix size must be greater than 5!!!\n");
                printf("Call the function -h\n");
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void check_o_1(int argc,char* argv,int* a,int m,int n)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-o", argv[i]) == 0)
        {
            a[n / 2] = m / 2;
            a[0] = m / 2;
            a[n - 1] = m / 2;
        }
    }
}

void swap_matrix(bitmap_t fruit, int j, int l, int* a)
{
    fill_matrix_black(fruit, j * l, fruit.height - a[j] * l, l);
    if (pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->red == 255 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->green == 0 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->blue == 0)
        fill_matrix_red(fruit, j * l, fruit.height - a[j] * l, l);

    if (pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->green == 255 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->red == 0 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->blue == 0)
        fill_matrix_green(fruit, j * l, fruit.height - a[j] * l, l);

    if (pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->blue == 255 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->green == 0 && pixel_at(&fruit, (j + 1) * l, fruit.height - a[j] * l)->red == 0)
        fill_matrix_blue(fruit, j * l, fruit.height - a[j] * l, l);

    fill_matrix_black(fruit, (j + 1) * l, fruit.height - a[j + 1] * l, l);
    if (pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->red == 255 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->green == 0 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->blue == 0)
        fill_matrix_red(fruit, (j + 1) * l, fruit.height - a[j + 1] * l, l);

    if (pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->green == 255 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->red == 0 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->blue == 0)
        fill_matrix_green(fruit, (j + 1) * l, fruit.height - a[j + 1] * l, l);

    if (pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->blue == 255 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->green == 0 && pixel_at(&fruit, j * l, fruit.height - a[j + 1] * l)->red == 0)
        fill_matrix_blue(fruit, (j + 1) * l, fruit.height - a[j + 1] * l, l);

    fill_matrix_white(fruit, (j + 1) * l, fruit.height - a[j] * l, l);

    fill_matrix_white(fruit, j * l, fruit.height - a[j + 1] * l, l);

    for (int k = fruit.height - a[j] * l - l; k > fruit.height - a[j + 1] * l; k = k - l)
        fill_matrix_white(fruit, j * l, k, l);

    for (int c = fruit.height - a[j + 1] * l + l; c < fruit.height - a[j] * l + l; c = c + l)
        fill_matrix_gray(fruit, (j + 1) * l, c, l);
}

int main(int argc, char **argv)
{
    bitmap_t fruit;
    
    int x = 0;
    int y = 0;
    int l = 3;

    fruit.width = 10;
    fruit.height = 10;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-h", argv[i]) == 0)
        {
            printf("Program for bubble sort visualization.\nGeneral options :\n- n[--number]\tNumber of values to sort\n- s[--size]\tBox size for each value(in pixels)\n- o[--order]\tDemonstrate stability or not\n- c[--complexity]\tPrint number of swaps / comparisions\n- h[--help]\tShow help\n\n$ bubblesort.exe -n 25 -s 10 -o -c\nDone!\n");
            printf("Post-scriptu:if the user does not use the matrix or pixel size input function, the default values are used\n");
            printf("default values:\nnumber = 10\nsize = 3");
            exit(EXIT_SUCCESS);
        }
        if (strcmp("-s", argv[i]) == 0)
        {
            if (i == argc - 1)
            {
                printf("Error\nSpecify the size of the pixel!!!");
                exit(EXIT_SUCCESS);
            }
            int ss = atoi(argv[i + 1]);
            if (ss >= 1)
            {
                l = ss;
            }
            else
            {
                printf("Error\n");
                printf("incorrect pixel size indication\n");
                printf("The pixel size must be greater than 1!!!\n");
                printf("Call the function -h\n");
                exit(EXIT_SUCCESS);
            }

        }
        if (strcmp("-n", argv[i]) == 0)
        {
            if (i == argc - 1)
            {
                printf("Error\nSpecify the size of the matrix!!!");
                exit(EXIT_SUCCESS);
            }
            int sn = atoi(argv[i + 1]);
            if (sn >= 5)
            {
                fruit.width = sn;
                fruit.height = sn;
            }
            else
            {
                printf("Error\n");
                printf("incorrect matrix size indication\n");
                printf("The matrix size must be greater than 5!!!\n");
                printf("Call the function -h\n");
                exit(EXIT_SUCCESS);
            }
        }
    }
    //check_help(argc, argv);
    //check_s(argc, argv, l);
    //check_n(argc, argv, fruit);

    fruit.width *= l;
    fruit.height *= l;

    fruit.pixels = calloc(sizeof(pixel_t), fruit.width * fruit.height);
    fill_matrix0(fruit, x, y, fruit.height, fruit.width);
    int m = fruit.height/l;
    int n = fruit.width/l;
    int* a = (int*)malloc(n * sizeof(int));;
    srand(time(NULL));
    fill_array(a, m, n);
    //print_array(a, n);
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-o", argv[i]) == 0)
        {
            a[n / 2] = m / 2;
            a[0] = m / 2;
            a[n - 1] = m / 2;
        }
    }
    //check_o_1(argc, argv, a, m, n);
    //printf("\n");
    //print_array(a, n);

    for (x = 0; x < fruit.width; x = x + l)
    {
        fill_matrix_black(fruit, x, fruit.height - (a[x / l] * l), l);

        for (int i = 1; i < argc; i++)
        {
            if (strcmp("-o", argv[i]) == 0)
            {
                if (x == 0)
                    fill_matrix_red(fruit, x, fruit.height - (a[x / l] * l), l);
                if (x == (m / 2) * l)
                    fill_matrix_green(fruit, x, fruit.height - (a[x / l] * l), l);

                if (x == fruit.height - l)
                    fill_matrix_blue(fruit, x, fruit.height - (a[x / l] * l), l);

            }
        }

        for (y = (fruit.height - a[x / l] * l) + l; y < fruit.height; y = y + l)
            fill_matrix_gray(fruit, x, y, l);

    }

    char str[100];

    int count = 1;
    sprintf(str, "frame%05d.png", count);
    count++;
    save_png_to_file(&fruit, str);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                swap(&a[j], &a[j + 1]);
                swap_matrix(fruit, j, l, a);

                sprintf(str, "frame%05d.png", count);
                save_png_to_file(&fruit, str);
                count++;
            }
        }
    }

    //printf("\n");
    //print_array(a, n);
    for (int i = 0; i < argc; i++)
    { 
        if(strcmp("-c", argv[i]) == 0)
        printf("\nQuantity of swap: %d\n", count - 2);
    }
    /*sprintf(str, "frame%05d.png", count);
    save_png_to_file(&fruit, str);*/
    printf("Done");

    free(a);
    free(fruit.pixels);
    return 0;
}
