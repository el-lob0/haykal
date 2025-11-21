#include <ft2build.h>
#include FT_FREETYPE_H
#include "../include/nib.h"
#include <string.h>




Pixel *bitmap_to_buffer(const FT_Bitmap *bmp) {
    if (!bmp || bmp->width == 0 || bmp->rows == 0)
        return NULL;

    if (bmp->pixel_mode != FT_PIXEL_MODE_GRAY) {
        return NULL;
    }

    Pixel *out = malloc(bmp->width * bmp->rows * sizeof(Pixel));
    if (!out) return NULL;

    for (unsigned int y = 0; y < bmp->rows; y++) {
        const unsigned char *src;

        if (bmp->pitch >= 0)
            src = bmp->buffer + y * bmp->pitch;
        else
            src = bmp->buffer + (bmp->rows - 1 - y) * (-bmp->pitch);

        for (unsigned int x = 0; x < bmp->width; x++) {
            float a = src[x] / 255.0f;

            out[y * bmp->width + x] = (Pixel){
                .r = 1.0f,
                .g = 1.0f,
                .b = 1.0f,
                .a = a
            };
        }
    }

    return out;
}

void freetype_char_to_rgba(const FT_Bitmap *bmp, Pixel *out) {
    int w = bmp->width;
    int h = bmp->rows;
    int pitch = bmp->pitch;

    for (int y = 0; y < h; y++) {
        const unsigned char *src =
            (pitch >= 0)
            ? bmp->buffer + y * pitch
            : bmp->buffer + (h - 1 - y) * (-pitch);

        for (int x = 0; x < w; x++) {
            unsigned char v = src[x];      
            float a = v / 255.0f;

            Pixel *p = &out[y * w + x];
            p->r = 1.0f;
            p->g = 1.0f;
            p->b = 1.0f;
            p->a = a;
        }
    }
}

FT_Face load_font(const char *path) {
  FT_Library library;
  FT_Face font;
  FT_New_Face(library, path, 0, &font);
}



// because i never remember how to loop on the 1d array
void loop(Pixel* buffer, int h, int w) {
  
  for (int y = 0; y < h; y++) {

    for (int x = 0; x < w; x++) {

      Pixel *p = &buffer[y * w + x]; 

    }
  }
}




typedef struct {
    Pixel **bitmaps;   // one Pixel* per glyph
    int *heights;
    int *advances;
} GlyphSet;



// has side effects on the `font` param

Pixel* process_string(const char *string, int font_size, FT_Face font, int width, int height) {
    int len = strlen(string);

    GlyphSet glyphs;
    glyphs.bitmaps  = malloc(sizeof(Pixel*) * len);
    glyphs.heights  = malloc(sizeof(int) * len);
    glyphs.advances = malloc(sizeof(int) * len);

    FT_Set_Pixel_Sizes(font, 0, font_size);

    int *glyph_width;

    for (int i = 0; i < len; i++) {
        FT_Load_Char(font, string[i], FT_LOAD_RENDER);

        FT_GlyphSlot slot = font->glyph;
        FT_Bitmap *bitmap = &slot->bitmap;

        int w = bitmap->width;
        int h = bitmap->rows;

        glyphs.heights[i]  = h;
        glyphs.advances[i] = slot->advance.x >> 6;
        glyph_width[i] = w;

        Pixel *px = malloc(sizeof(Pixel) * w * h);
        glyphs.bitmaps[i] = px;

        unsigned char *buf = bitmap->buffer;

        if (w == 0 || h == 0 || buf == NULL)
            continue;

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int i = y * w + x;
                unsigned char a = buf[i];  

                px[i].r = 1.0;
                px[i].g = 1.0;
                px[i].b = 1.0;
                px[i].a = a / 255.0;
            }
        }
    }

    int line_w = 0;
    int line_h = 0;

    for (int i = 0; i < len; i++) {
        line_w += glyphs.advances[i];
        if (glyphs.heights[i] > line_h)
            line_h = glyphs.heights[i];
    }
    
    Pixel *out = nib_init_buffer(line_w, line_h);
    
    int cursor_position = 0; int i;

    for (i=0; i< len; i++) {
      int gw = glyph_width[i];
      int gh = glyphs.heights[i];
      Pixel *src = glyphs.bitmaps[i];
      int y; int x;

      for (y = 0; y<gh; y++) {
        for (x = 0; x<gw; x++) {
          int destination_y = cursor_position + x;
          int destination_x = y; 
          
          int destination_i = destination_y * line_w + destination_x;
          int src_i = y * gw + x;
          
          out[destination_i] = src[src_i];
        }
      }
      cursor_position += glyphs.advances[i];
    }
    

    return out; // you will later build the merged-output buffer
}

