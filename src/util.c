#include <ft2build.h>
#include FT_FREETYPE_H
#include <string.h>
#include <stdlib.h>
#include "../include/nib.h"

typedef struct {
    Pixel *buffer;
    int width;
    int height;
} TextBitmap;


/*
 
1. Init FreeType + load font (once)

2. Split input text by '\n'
   (optional: implement word wrapping)

3. For each line:
     For each codepoint:
        FT_Load_Char(...)
        Store glyph metrics + alpha bitmap

4. Compute final overall bitmap size:
       width  = max(sum of advances for each line)
       height = line_count * line_height

5. Allocate final Pixel buffer

6. For each glyph in each line:
       Compute destination position
       Blit glyph.alpha → Pixel rgba into final buffer

7. Store final buffer in the element for rendering

 */















void load_font(char* filepath, int font_size) {
  FT_Library font_lib;
  FT_Init_FreeType(&font_lib);

  FT_Face font_face;
  FT_New_Face(font_lib, filepath, 0, &font_face);
  FT_Set_Pixel_Sizes(font_face, 0, font_size);

  int padding = 2;
  int row = 0;
  int col padding;

  const texture_width = 512;
  char text_buffer[texture_width * texture_width];
  for (FT_ULong glyphIdx=32; glyphIdx<127; glyphIdx++) {
    FT_UInt glyph_index = FT_Get_Char_Index(font_face, glyphIdx);
    FT_Load_Glyph(font_face, glyph_index, FT_LOAD_DEFAULT);
    FT_Error error = FT_Render_Glyph(font_face->glyph, FT_TENDER_MODE_NORMAL);

    if (col + font_face->glyph->bitmap.width + padding >= 512) {
      col = padding;
      row += font_size;
    }



  }

}

