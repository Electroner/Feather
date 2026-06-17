#include "../ImageWork.h"
#include <cstring>

// Fully self-contained glyph record — no raw pointer aliasing
struct GlyphRecord {
	int advance      = 0;
	int bitmap_left  = 0;
	int bitmap_top   = 0;
	int w            = 0;
	int h            = 0;
	std::vector<unsigned char> pixels; // grayscale bitmap data (w*h bytes)
};

ImageStr rasterText(const char* text, const char* fontPath, int size, const RGB& color)
{
	ImageStr word; // default: data=NULL, width=0, height=0

	if (!text || !fontPath || text[0] == '\0' || size <= 0) return word;

	FT_Library library;
	if (FT_Init_FreeType(&library)) return word;

	FT_Face face;
	if (FT_New_Face(library, fontPath, 0, &face)) {
		FT_Done_FreeType(library);
		return word;
	}

	FT_Set_Pixel_Sizes(face, 0, (FT_UInt)size);

	// ---- Collect per-glyph metrics ----
	std::vector<GlyphRecord> glyphs;
	int ascent  = 0; // max pixels above baseline
	int descent = 0; // max pixels below baseline

	for (int i = 0; text[i]; i++) {
		unsigned char c = (unsigned char)text[i];

		// Skip control characters (newline, tab, etc.)
		if (c < 32) {
			glyphs.push_back(GlyphRecord{});
			continue;
		}

		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			// Character not in font — push zero-advance placeholder
			glyphs.push_back(GlyphRecord{});
			continue;
		}

		FT_GlyphSlot slot = face->glyph;
		GlyphRecord g;
		g.advance     = (int)(slot->advance.x >> 6);
		g.bitmap_left = slot->bitmap_left;
		g.bitmap_top  = slot->bitmap_top;
		g.w           = (int)slot->bitmap.width;
		g.h           = (int)slot->bitmap.rows;

		if (g.w > 0 && g.h > 0 && slot->bitmap.buffer) {
			g.pixels.assign(slot->bitmap.buffer,
			                slot->bitmap.buffer + g.w * g.h);
		}

		if (g.bitmap_top > ascent) ascent = g.bitmap_top;
		int below = g.h - g.bitmap_top;
		if (below > descent) descent = below;

		glyphs.push_back(std::move(g));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	if (glyphs.empty()) return word;

	// ---- Compute canvas size ----
	// Width = sum of all advances except last + last glyph's right edge
	int penX = 0;
	for (int i = 0; i < (int)glyphs.size() - 1; i++)
		penX += glyphs[i].advance;
	// Last glyph's right edge relative to its pen position
	const GlyphRecord& last = glyphs.back();
	int totalW = penX + last.bitmap_left + last.w;
	if (totalW <= 0) totalW = penX + last.advance; // fallback
	int totalH = ascent + descent;

	if (totalW <= 0 || totalH <= 0) return word;

	// ---- Allocate output image (transparent) ----
	word.width    = totalW;
	word.height   = totalH;
	word.channels = 4;
	word.name     = text;
	word.extension = "png";
	word.loaded   = true;
	word.modified = false;
	word.data     = new unsigned char[totalW * totalH * 4]();  // zero-init

	float fr = color.r     / 255.0f;
	float fg = color.g     / 255.0f;
	float fb = color.b     / 255.0f;
	float fa = color.delta / 255.0f;

	// ---- Blit each glyph onto the canvas ----
	penX = 0;
	for (const auto& g : glyphs) {
		if (!g.pixels.empty() && g.w > 0 && g.h > 0) {
			int gx = penX + g.bitmap_left;
			int gy = ascent - g.bitmap_top;

			for (int row = 0; row < g.h; row++) {
				for (int col = 0; col < g.w; col++) {
					int dx = gx + col;
					int dy = gy + row;
					// Hard bounds check — prevents every possible overflow
					if (dx < 0 || dx >= totalW || dy < 0 || dy >= totalH) continue;
					int idx = (dy * totalW + dx) * 4;
					unsigned char lum = g.pixels[row * g.w + col];
					word.data[idx    ] = (unsigned char)(lum * fr);
					word.data[idx + 1] = (unsigned char)(lum * fg);
					word.data[idx + 2] = (unsigned char)(lum * fb);
					word.data[idx + 3] = (unsigned char)(lum * fa);
				}
			}
		}
		penX += g.advance;
	}

	return word;
}

void ImageWork::toolText(int, int) {
	// Text creation is handled by FeatherGUI via createTextLayer() / renderTextToLayer().
}
