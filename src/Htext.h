#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include <stdlib.h>
#include <string.h>
#include "../include/nib.h"

#include "freetype/freetype.h"


// ----------------- WRAPPER ------------------------------

typedef struct {
  int width;
  int height;
  int bearing_x;
  int bearing_y;
  int advance;
  int atlas_x;
  int atlas_y;
  unsigned char *alpha;
} Glyph;

typedef struct {
  FT_Library lib;
  FT_Face font;
  int size;
} Font;


int Hfont_init(Font *font, const char *path, int px) {
    if (FT_Init_FreeType(&font->lib))
        return 0;

    if (FT_New_Face(font->lib, path, 0, &font->font))
        return 0;

    FT_Set_Pixel_Sizes(font->font, 0, px);
    font->size = px;

    return 1;
}


Glyph Hfont_load_glyph(Font *font, unsigned long c) {
    Glyph g = {0};

    FT_Load_Char(font->font, c, FT_LOAD_RENDER);
    FT_GlyphSlot slot = font->font->glyph;
    FT_Bitmap *bm = &slot->bitmap;

    g.width     = bm->width;
    g.height    = bm->rows;
    g.bearing_x = slot->bitmap_left;
    g.bearing_y = slot->bitmap_top;
    g.advance   = slot->advance.x >> 6;

    if (g.width && g.height) {
        g.alpha = malloc(g.width * g.height);
        for (int y = 0; y < g.height; y++) {
            memcpy(
                g.alpha + y * g.width,
                bm->buffer + y * bm->pitch,
                g.width
            );
        }
    }

    return g;
}

void Hfont_destroy(Font *font) {
    FT_Done_Face(font->font);
    FT_Done_FreeType(font->lib);
}

typedef struct {
    int atlas_w;
    int atlas_h;
    unsigned char *alpha;  

    Glyph glyphs[256]; 
    int count;
} Atlas;

// ------------------ HELPERS --------------------------------
#define max(a, b) ((a) > (b) ? (a) : (b))

Atlas Hbuild_atlas(Font *font) {
  Glyph glyph_arr[127];

  int atlas_w = 0;
  int atlas_h = 0;


  int i=32;
  // ascii table values. enough for now
  for (char c = 32; c < 127; c++ ) {
    Glyph g = Hfont_load_glyph(font, c);

    atlas_w += g.width + 1;
    atlas_h = max(g.height, atlas_h);
    
    glyph_arr[i] = g;

    i++;

  }

  unsigned char *alpha = calloc(atlas_w * atlas_h, 1);
  int pen_x = 0;
  for (char c = 32; c < 127; c++) {

      Glyph *g = &glyph_arr[(int)c];

      g->atlas_x = pen_x;
      g->atlas_y = 0;

      for (int y = 0; y < g->height; y++) {
          memcpy(
              alpha + y * atlas_w + pen_x,
              g->alpha + y * g->width,
              g->width
          );
      }

      pen_x += g->width + 1;

      free(g->alpha);
      g->alpha = NULL;
  }

  Atlas atlas = {0};
  atlas.atlas_w = atlas_w;
  atlas.atlas_h = atlas_h;
  atlas.alpha = alpha;
  memcpy(atlas.glyphs, glyph_arr, sizeof(glyph_arr));
  atlas.count = 127 - 32;

  return atlas;
}


int Htext_width(Atlas *atlas, const char *text) {
  int w = 0;
  for (const char *p = text; *p; p++) {
    Glyph *g = &atlas->glyphs[(unsigned char)*p];
    w += g->advance;
  }
  return w;
}

void flip_atlas(Atlas *atlas) {
    int w = atlas->atlas_w;
    int h = atlas->atlas_h;
    for (int y = 0; y < h / 2; y++) {
        unsigned char *top = atlas->alpha + y * w;
        unsigned char *bottom = atlas->alpha + (h - 1 - y) * w;

        for (int x = 0; x < w; x++) {
            unsigned char tmp = top[x];
            top[x] = bottom[x];
            bottom[x] = tmp;
        }
    }

    // after flipping the atlas, adjust each glyph's y-coordinate
    for (int i = 32; i < 127; i++) {
        Glyph *g = &atlas->glyphs[i];
        g->atlas_y = h - g->atlas_y - g->height;
    }
}



void Hrender_text(Atlas *atlas, const char *text, Pixel *buffer, int buffer_w, int buffer_h, Pixel color, int font_size) {
    flip_atlas(atlas);
    int x = 0, y = 0;

    int baseline = 0;
    for (int i = 32; i < 127; i++) {
        baseline = max(baseline, atlas->glyphs[i].bearing_y);
    }

    for (const char *byte = text; *byte; byte++) {
    
        if (*byte == '\n') { x = 0; y += font_size; continue; }

        Glyph *g = &atlas->glyphs[(unsigned char)*byte];

        for (int gy = 0; gy < g->height; gy++) {

            for (int gx = 0; gx < g->width; gx++) {
                int dst_x = x + gx + g->bearing_x;
                int dst_y = y + gy + (atlas->glyphs['A'].bearing_y - g->bearing_y); // align baseline
                if (dst_x < 0 || dst_x >= buffer_w || dst_y < 0 || dst_y >= buffer_h) continue;

                float a = atlas->alpha[(g->atlas_y + gy) * atlas->atlas_w + (g->atlas_x + gx)] / 255.0f;
                Pixel *p = &buffer[dst_y * buffer_w + dst_x];
                p->r = color.r * a;
                p->g = color.g * a;
                p->b = color.b * a;
                p->a = a;
            }
        }

        x += g->advance;
    }
}































