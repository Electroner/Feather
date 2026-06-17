#include "./ImageWork.h"

void ImageWork::init() {

	//set to false every loaded image in the vector
	for (int i = 0; i < (int)this->Images.size(); i++) {
		Images.at(i).loaded = false;
	}

	this->activeLayerIndex = 0;
	this->canvasWidth  = 0;
	this->canvasHeight = 0;
	this->CavasSize = std::make_pair(-1, -1);
	this->toolradius = 5;

	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);

	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);

	this->selectionEnable = false;
	this->selectionDone = false;
	this->textEnable = false;

	//Colors
	this->toolColor = RGB(0, 0, 0, 255);
	this->secondaryColor = RGB(255, 255, 255, 255);

	//First point of the selection to -1
	this->firstPoint = std::pair<int, int>(-1, -1);
}

void ImageWork::addImage(ImageStr& _image) {
	//For the image create a Opengl texture
	if (_image.data != NULL) {
		//Enable transparency
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Create the texture
		glGenTextures(1, &_image.texture);
		glBindTexture(GL_TEXTURE_2D, _image.texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (_image.channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image.width, _image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.data);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.data);
		}
		else if (_image.channels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image.width, _image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.data);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.data);
		}
		//Set the image as loaded
		_image.loaded = true;

		//Add the image to the vector
		this->Images.push_back(_image);
	}
}

void ImageWork::combineLayers() {
	if (!Images.empty()) {
		if (activeLayerIndex < 0 || activeLayerIndex >= (int)Images.size())
			activeLayerIndex = 0;
		this->CurrentImage = this->Images[activeLayerIndex];
	}
}

// ---- Layer engine -------------------------------------------------------

void ImageWork::setActiveLayer(int index) {
	if (index >= 0 && index < (int)Images.size()) {
		activeLayerIndex = index;
		CurrentImage = Images[activeLayerIndex];
	}
}

int ImageWork::getActiveLayerIndex() {
	return activeLayerIndex;
}

ImageStr* ImageWork::getCompositeImage() {
	return &CompositeImage;
}

int ImageWork::getCanvasWidth() {
	return (canvasWidth > 0) ? canvasWidth : (!Images.empty() ? Images[0].width : 0);
}

int ImageWork::getCanvasHeight() {
	return (canvasHeight > 0) ? canvasHeight : (!Images.empty() ? Images[0].height : 0);
}

void ImageWork::setCanvasSize(int w, int h) {
	if (w > 0) canvasWidth  = w;
	if (h > 0) canvasHeight = h;
}

void ImageWork::resizeCanvas(int newW, int newH, int anchorX, int anchorY) {
	if (newW <= 0 || newH <= 0 || Images.empty()) return;

	int oldW = getCanvasWidth();
	int oldH = getCanvasHeight();

	// Compute pixel offset for the anchor (how much existing content shifts)
	int offX = 0, offY = 0;
	if (anchorX == 1) offX = (newW - oldW) / 2;
	else if (anchorX == 2) offX = newW - oldW;

	if (anchorY == 1) offY = (newH - oldH) / 2;
	else if (anchorY == 2) offY = newH - oldH;

	for (auto& img : Images) {
		unsigned char* nd = new unsigned char[newW * newH * 4]();

		int sxs = (-offX > 0) ? -offX : 0;
		int sys = (-offY > 0) ? -offY : 0;
		int sxe = (img.width  < newW - offX) ? img.width  : newW - offX;
		int sye = (img.height < newH - offY) ? img.height : newH - offY;

		for (int sy = sys; sy < sye; sy++) {
			for (int sx = sxs; sx < sxe; sx++) {
				int dx = sx + offX;
				int dy = sy + offY;
				if (dx < 0 || dx >= newW || dy < 0 || dy >= newH) continue;
				int si = (sy * img.width  + sx) * img.channels;
				int di = (dy * newW + dx) * 4;
				nd[di    ] = img.data[si    ];
				nd[di + 1] = img.data[si + 1];
				nd[di + 2] = img.data[si + 2];
				nd[di + 3] = (img.channels == 4) ? img.data[si + 3] : 255;
			}
		}

		delete[] img.data;
		img.data     = nd;
		img.width    = newW;
		img.height   = newH;
		img.channels = 4;
		img.modified = true;

		glBindTexture(GL_TEXTURE_2D, img.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newW, newH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nd);
	}

	canvasWidth  = newW;
	canvasHeight = newH;

	// Force composite reallocation
	delete[] CompositeImage.data;
	CompositeImage.data   = nullptr;
	CompositeImage.width  = 0;
	CompositeImage.height = 0;
}

// ---- Performance helpers -----------------------------------------------

void ImageWork::resetInterpolationBounds() {
	int w = getCanvasWidth(), h = getCanvasHeight();
	interpolationMin = { w, h };
	interpolationMax = { 0, 0 };
}

void ImageWork::updateDirtyRect(int x, int y, int r) {
	if (x - r < interpolationMin.first)  interpolationMin.first  = x - r;
	if (y - r < interpolationMin.second) interpolationMin.second = y - r;
	if (x + r > interpolationMax.first)  interpolationMax.first  = x + r;
	if (y + r > interpolationMax.second) interpolationMax.second = y + r;
}

void ImageWork::clampDirtyRect() {
	int cw = getCanvasWidth(), ch = getCanvasHeight();
	if (interpolationMin.first  < 0)  interpolationMin.first  = 0;
	if (interpolationMin.second < 0)  interpolationMin.second = 0;
	if (interpolationMax.first  >= cw) interpolationMax.first  = cw - 1;
	if (interpolationMax.second >= ch) interpolationMax.second = ch - 1;
}

// Precomputed circle stamp: built once per radius, reused every stroke step
struct CircleStamp {
	int radius = -1;
	// offset pairs (dx, dy) inside the disk
	std::vector<short> dx, dy;
	// radial alpha (255 at center, 0 at edge) — used by soft brush
	std::vector<unsigned char> alpha;
};
static CircleStamp g_stamp;

static void rebuildStamp(int r) {
	if (g_stamp.radius == r) return;
	g_stamp.radius = r;
	g_stamp.dx.clear(); g_stamp.dy.clear(); g_stamp.alpha.clear();
	int r2 = r * r;
	for (int dy = -r; dy <= r; dy++) {
		for (int dx = -r; dx <= r; dx++) {
			int d2 = dx*dx + dy*dy;
			if (d2 <= r2) {
				g_stamp.dx.push_back((short)dx);
				g_stamp.dy.push_back((short)dy);
				// alpha: 255 at center, 0 at rim
				int a = (r2 > 0) ? 255 - (d2 * 255) / r2 : 255;
				g_stamp.alpha.push_back((unsigned char)a);
			}
		}
	}
}

void ImageWork::fastStroke(int x1, int y1, int x2, int y2, int radius, BrushMode mode) {
	ImageStr* img = getImageStrP();
	if (!img || !img->data) return;

	unsigned char* data = img->data;
	const int W = img->width, H = img->height;

	rebuildStamp(radius);

	RGB color = (mode == BRUSH_ERASE) ? secondaryColor : toolColor;
	const int N = (int)g_stamp.dx.size();

	// Bresenham line from (x1,y1) to (x2,y2)
	int adx = (x2 > x1) ? x2 - x1 : x1 - x2;
	int ady = (y2 > y1) ? y2 - y1 : y1 - y2;
	int sx  = (x1 < x2) ? 1 : -1;
	int sy  = (y1 < y2) ? 1 : -1;
	int err = adx - ady;

	while (true) {
		// Stamp disk at current (x1, y1)
		if (mode == BRUSH_SOFT) {
			int baseAlpha = color.delta; // tool alpha
			for (int k = 0; k < N; k++) {
				int px = x1 + g_stamp.dx[k];
				int py = y1 + g_stamp.dy[k];
				if ((unsigned)px >= (unsigned)W || (unsigned)py >= (unsigned)H) continue;
				int idx = (py * W + px) * 4;
				// sa = radial_alpha * tool_alpha / 255
				int sa  = (g_stamp.alpha[k] * baseAlpha) >> 8;
				int isa = 255 - sa;
				data[idx    ] = (unsigned char)((data[idx    ] * isa + color.r * sa) >> 8);
				data[idx + 1] = (unsigned char)((data[idx + 1] * isa + color.g * sa) >> 8);
				data[idx + 2] = (unsigned char)((data[idx + 2] * isa + color.b * sa) >> 8);
				data[idx + 3] = (unsigned char)(data[idx + 3] + ((sa * (255 - data[idx + 3])) >> 8));
			}
		} else {
			// Hard / Erase: direct write
			for (int k = 0; k < N; k++) {
				int px = x1 + g_stamp.dx[k];
				int py = y1 + g_stamp.dy[k];
				if ((unsigned)px >= (unsigned)W || (unsigned)py >= (unsigned)H) continue;
				int idx = (py * W + px) * 4;
				data[idx    ] = color.r;
				data[idx + 1] = color.g;
				data[idx + 2] = color.b;
				data[idx + 3] = color.delta;
			}
		}

		if (x1 == x2 && y1 == y2) break;
		int e2 = 2 * err;
		if (e2 > -ady) { err -= ady; x1 += sx; }
		if (e2 <  adx) { err += adx; y1 += sy; }
	}

	img->modified = true;
}

// ---- Compositing -------------------------------------------------------

// Core region-blending kernel (integer Porter-Duff "src over dst")
// Works on a sub-rectangle [x0, y0, x0+rw, y0+rh] of cw-wide buffers.
static void blendRegion(unsigned char* comp, const std::vector<ImageStr>& layers,
                        int x0, int y0, int rw, int rh, int cw)
{
	// Count visible layers for fast paths
	int visCount = 0, visIdx = -1;
	for (int i = 0; i < (int)layers.size(); i++) {
		if (layers[i].visible && layers[i].data) { visCount++; visIdx = i; }
	}

	if (visCount == 0) {
		for (int y = y0; y < y0 + rh; y++)
			memset(comp + (y * cw + x0) * 4, 0, rw * 4);
		return;
	}

	if (visCount == 1 && layers[visIdx].opacity >= 0.9999f) {
		// Single fully-opaque layer: straight copy
		const unsigned char* src = layers[visIdx].data;
		for (int y = y0; y < y0 + rh; y++)
			memcpy(comp + (y * cw + x0) * 4, src + (y * cw + x0) * 4, rw * 4);
		return;
	}

	// General: clear then blend bottom-to-top
	for (int y = y0; y < y0 + rh; y++)
		memset(comp + (y * cw + x0) * 4, 0, rw * 4);

	for (int i = (int)layers.size() - 1; i >= 0; i--) {
		const ImageStr& L = layers[i];
		if (!L.visible || !L.data) continue;
		int opI = (int)(L.opacity * 256.0f);
		if (opI <= 0) continue;

		const unsigned char* src = L.data;
		for (int y = y0; y < y0 + rh; y++) {
			const unsigned char* srow = src  + (y * cw + x0) * 4;
			unsigned char*       crow = comp + (y * cw + x0) * 4;
			int xi = rw;
			while (xi--) {
				int sa = (srow[3] * opI) >> 8;
				if (sa) {
					int isa = 255 - sa;
					crow[0] = (unsigned char)((srow[0] * sa + crow[0] * isa) >> 8);
					crow[1] = (unsigned char)((srow[1] * sa + crow[1] * isa) >> 8);
					crow[2] = (unsigned char)((srow[2] * sa + crow[2] * isa) >> 8);
					crow[3] = (unsigned char)(sa + (crow[3] * isa >> 8));
				}
				srow += 4; crow += 4;
			}
		}
	}
}

void ImageWork::compositeRegion(int x0, int y0, int rw, int rh) {
	if (Images.empty()) return;
	int cw = getCanvasWidth(), ch = getCanvasHeight();
	if (cw <= 0 || ch <= 0) return;

	// Allocate / reallocate composite buffer
	bool newTexture = false;
	if (!CompositeImage.data || CompositeImage.width != cw || CompositeImage.height != ch) {
		delete[] CompositeImage.data;
		CompositeImage.data     = new unsigned char[cw * ch * 4];
		CompositeImage.width    = cw;
		CompositeImage.height   = ch;
		CompositeImage.channels = 4;
		newTexture = true;
		x0 = 0; y0 = 0; rw = cw; rh = ch; // force full init
	}

	// Clamp to canvas
	if (x0 < 0)        { rw += x0; x0 = 0; }
	if (y0 < 0)        { rh += y0; y0 = 0; }
	if (x0 + rw > cw)  rw = cw - x0;
	if (y0 + rh > ch)  rh = ch - y0;
	if (rw <= 0 || rh <= 0) return;

	blendRegion(CompositeImage.data, Images, x0, y0, rw, rh, cw);

	// Upload to GPU
	if (CompositeImage.texture == 0 || newTexture) {
		if (CompositeImage.texture == 0) glGenTextures(1, &CompositeImage.texture);
		glBindTexture(GL_TEXTURE_2D, CompositeImage.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cw, ch, 0, GL_RGBA, GL_UNSIGNED_BYTE, CompositeImage.data);
	} else if (rw == cw && rh == ch) {
		glBindTexture(GL_TEXTURE_2D, CompositeImage.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cw, ch, 0, GL_RGBA, GL_UNSIGNED_BYTE, CompositeImage.data);
	} else {
		glBindTexture(GL_TEXTURE_2D, CompositeImage.texture);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, cw);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x0, y0, rw, rh, GL_RGBA, GL_UNSIGNED_BYTE,
		                CompositeImage.data + (y0 * cw + x0) * 4);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
}

void ImageWork::compositeAllLayers() {
	int cw = getCanvasWidth(), ch = getCanvasHeight();
	if (cw > 0 && ch > 0 && !Images.empty())
		compositeRegion(0, 0, cw, ch);
}

void ImageWork::insertImageOnTop(ImageStr _image) {
	if (canvasWidth == 0 || canvasHeight == 0) {
		canvasWidth  = _image.width;
		canvasHeight = _image.height;
	}
	Images.insert(Images.begin(), _image);
	activeLayerIndex = 0;
	CurrentImage = Images[0];
}

void ImageWork::addNewLayer(const std::string& name) {
	if (Images.empty()) return;

	int w = Images[0].width;
	int h = Images[0].height;

	ImageStr layer;
	layer.name = name.empty() ? ("Layer " + std::to_string(Images.size() + 1)) : name;
	layer.width = w;
	layer.height = h;
	layer.channels = 4;
	layer.extension = "png";
	layer.modified = false;
	layer.loaded = true;
	layer.visible = true;
	layer.opacity = 1.0f;

	layer.data = new unsigned char[w * h * 4];
	memset(layer.data, 0, w * h * 4); // transparent

	glGenTextures(1, &layer.texture);
	glBindTexture(GL_TEXTURE_2D, layer.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, layer.data);

	Images.insert(Images.begin(), layer);
	activeLayerIndex = 0;
	CurrentImage = Images[0];
}

void ImageWork::removeLayer(int index) {
	if (index < 0 || index >= (int)Images.size()) return;
	if ((int)Images.size() == 1) return; // keep at least one layer

	glDeleteTextures(1, &Images[index].texture);
	delete[] Images[index].data;
	Images[index].data = nullptr;
	Images.erase(Images.begin() + index);

	if (activeLayerIndex >= (int)Images.size())
		activeLayerIndex = (int)Images.size() - 1;

	CurrentImage = Images[activeLayerIndex];
}

void ImageWork::duplicateLayer(int index) {
	if (index < 0 || index >= (int)Images.size()) return;

	ImageStr copy = Images[index];
	copy.name = Images[index].name + " copy";

	int size = copy.width * copy.height * copy.channels;
	copy.data = new unsigned char[size];
	memcpy(copy.data, Images[index].data, size);

	glGenTextures(1, &copy.texture);
	glBindTexture(GL_TEXTURE_2D, copy.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, copy.width, copy.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, copy.data);

	// Insert above the source layer
	Images.insert(Images.begin() + index, copy);
	activeLayerIndex = index;
	CurrentImage = Images[activeLayerIndex];
}

void ImageWork::mergeLayerDown(int index) {
	// Merges Images[index] (top) into Images[index+1] (below it)
	if (index < 0 || index >= (int)Images.size() - 1) return;

	ImageStr& top    = Images[index];
	ImageStr& bottom = Images[index + 1];

	int w = bottom.width;
	int h = bottom.height;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int idx = (y * w + x) * 4;
			if (x >= top.width || y >= top.height) continue;

			float srcR = top.data[idx    ] / 255.0f;
			float srcG = top.data[idx + 1] / 255.0f;
			float srcB = top.data[idx + 2] / 255.0f;
			float srcA = (top.channels == 4 ? top.data[idx + 3] / 255.0f : 1.0f) * top.opacity;

			float dstR = bottom.data[idx    ] / 255.0f;
			float dstG = bottom.data[idx + 1] / 255.0f;
			float dstB = bottom.data[idx + 2] / 255.0f;
			float dstA = (bottom.channels == 4 ? bottom.data[idx + 3] / 255.0f : 1.0f);

			float outA = srcA + dstA * (1.0f - srcA);
			if (outA > 0.0f) {
				bottom.data[idx    ] = (unsigned char)(((srcR * srcA + dstR * dstA * (1.0f - srcA)) / outA) * 255.0f);
				bottom.data[idx + 1] = (unsigned char)(((srcG * srcA + dstG * dstA * (1.0f - srcA)) / outA) * 255.0f);
				bottom.data[idx + 2] = (unsigned char)(((srcB * srcA + dstB * dstA * (1.0f - srcA)) / outA) * 255.0f);
			}
			bottom.data[idx + 3] = (unsigned char)(outA * 255.0f);
		}
	}

	// Refresh bottom layer texture
	glBindTexture(GL_TEXTURE_2D, bottom.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom.data);

	// Remove the top layer
	glDeleteTextures(1, &top.texture);
	delete[] top.data;
	top.data = nullptr;
	Images.erase(Images.begin() + index);

	if (activeLayerIndex >= (int)Images.size())
		activeLayerIndex = (int)Images.size() - 1;

	CurrentImage = Images[activeLayerIndex];
}

void ImageWork::flattenAllLayers() {
	if (Images.empty()) return;

	compositeAllLayers();

	int w = CompositeImage.width;
	int h = CompositeImage.height;

	// Free all current layers
	for (auto& img : Images) {
		glDeleteTextures(1, &img.texture);
		delete[] img.data;
		img.data = nullptr;
	}
	Images.clear();

	// Single flat layer from composite
	ImageStr flat;
	flat.name = "Background";
	flat.width = w;
	flat.height = h;
	flat.channels = 4;
	flat.extension = "png";
	flat.modified = true;
	flat.loaded = true;
	flat.visible = true;
	flat.opacity = 1.0f;

	flat.data = new unsigned char[w * h * 4];
	memcpy(flat.data, CompositeImage.data, w * h * 4);

	glGenTextures(1, &flat.texture);
	glBindTexture(GL_TEXTURE_2D, flat.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flat.data);

	Images.push_back(flat);
	activeLayerIndex = 0;
	CurrentImage = Images[0];
}

// ---- Text layer ---------------------------------------------------------

void ImageWork::createTextLayer(const std::string& text, int x, int y, int fontSize, const RGB& color) {
	int cw = getCanvasWidth();
	int ch = getCanvasHeight();
	if (cw <= 0 || ch <= 0) return;

	ImageStr layer;
	layer.name        = "Text";
	layer.width       = cw;
	layer.height      = ch;
	layer.channels    = 4;
	layer.extension   = "png";
	layer.modified    = false;
	layer.loaded      = true;
	layer.visible     = true;
	layer.opacity     = 1.0f;
	layer.isTextLayer = true;
	layer.textContent = text;
	layer.textFontSize = fontSize;
	layer.textAnchorX = x;
	layer.textAnchorY = y;
	layer.textColor   = color;
	layer.textRenderW = 0;
	layer.textRenderH = 0;

	layer.data = new unsigned char[cw * ch * 4]();

	glGenTextures(1, &layer.texture);
	glBindTexture(GL_TEXTURE_2D, layer.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cw, ch, 0, GL_RGBA, GL_UNSIGNED_BYTE, layer.data);

	Images.insert(Images.begin(), layer);
	activeLayerIndex = 0;
	CurrentImage = Images[0];

	if (!text.empty())
		renderTextToLayer(0);
}

void ImageWork::renderTextToLayer(int layerIdx) {
	if (layerIdx < 0 || layerIdx >= (int)Images.size()) return;
	ImageStr& layer = Images[layerIdx];
	if (!layer.isTextLayer || !layer.data) return;

	int cw = layer.width;
	int ch = layer.height;

	memset(layer.data, 0, cw * ch * 4);
	layer.textRenderW = 0;
	layer.textRenderH = 0;

	if (!layer.textContent.empty()) {
		// Locate a usable font
		std::string fontPath;
		{
			namespace fs = std::filesystem;
			std::string fa = fontsPath + "\\Arial.ttf";
			std::string fb = fontsPath + "\\calibri.ttf";
			std::string fc = fontsPath + "\\Consolas.ttf";
			if      (fs::exists(fa)) fontPath = fa;
			else if (fs::exists(fb)) fontPath = fb;
			else if (fs::exists(fc)) fontPath = fc;
			else                     fontPath = "./resources/fonts/consolas.ttf";
			std::replace(fontPath.begin(), fontPath.end(), '\\', '/');
		}

		ImageStr tmp = rasterText(layer.textContent.c_str(), fontPath.c_str(),
		                          layer.textFontSize, layer.textColor);

		if (tmp.data && tmp.width > 0 && tmp.height > 0) {
			layer.textRenderW = tmp.width;
			layer.textRenderH = tmp.height;

			for (int y = 0; y < tmp.height; y++) {
				for (int x = 0; x < tmp.width; x++) {
					int dx = layer.textAnchorX + x;
					int dy = layer.textAnchorY + y;
					if (dx < 0 || dx >= cw || dy < 0 || dy >= ch) continue;
					int si = (y * tmp.width + x) * 4;
					int di = (dy * cw + dx) * 4;
					layer.data[di    ] = tmp.data[si    ];
					layer.data[di + 1] = tmp.data[si + 1];
					layer.data[di + 2] = tmp.data[si + 2];
					layer.data[di + 3] = tmp.data[si + 3];
				}
			}
			delete[] tmp.data;
		}
	}

	glBindTexture(GL_TEXTURE_2D, layer.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cw, ch, 0, GL_RGBA, GL_UNSIGNED_BYTE, layer.data);
	layer.modified = true;
}

int ImageWork::findTextLayerAt(int x, int y) const {
	const int MARGIN = 12;
	for (int i = 0; i < (int)Images.size(); i++) {
		const ImageStr& img = Images[i];
		if (!img.isTextLayer || !img.visible) continue;
		int rw = (img.textRenderW > 0) ? img.textRenderW : img.textFontSize * 5;
		int rh = (img.textRenderH > 0) ? img.textRenderH : img.textFontSize * 2;
		if (x >= img.textAnchorX - MARGIN && x <= img.textAnchorX + rw + MARGIN &&
		    y >= img.textAnchorY - MARGIN && y <= img.textAnchorY + rh + MARGIN)
			return i;
	}
	return -1;
}

void ImageWork::moveLayer(int from, int to) {
	if (from == to) return;
	if (from < 0 || from >= (int)Images.size()) return;
	if (to   < 0 || to   >= (int)Images.size()) return;

	ImageStr moved = Images[from];
	Images.erase(Images.begin() + from);
	Images.insert(Images.begin() + to, moved);

	// Keep activeLayerIndex pointing at the same logical layer
	if (activeLayerIndex == from) {
		activeLayerIndex = to;
	} else if (from < to) {
		if (activeLayerIndex > from && activeLayerIndex <= to) activeLayerIndex--;
	} else {
		if (activeLayerIndex >= to && activeLayerIndex < from) activeLayerIndex++;
	}
	CurrentImage = Images[activeLayerIndex];
}

// ---- Undo / Redo -------------------------------------------------------

void ImageWork::buildUndoState(UndoState& out) const {
	out.activeLayerIndex = activeLayerIndex;
	out.canvasWidth      = canvasWidth;
	out.canvasHeight     = canvasHeight;
	out.layers.clear();
	for (const auto& img : Images) {
		UndoLayerData ld;
		ld.name      = img.name;
		ld.extension = img.extension;
		ld.width     = img.width;
		ld.height    = img.height;
		ld.channels  = img.channels;
		ld.visible   = img.visible;
		ld.opacity   = img.opacity;
		if (img.data && img.width > 0 && img.height > 0)
			ld.data.assign(img.data, img.data + img.width * img.height * img.channels);
		out.layers.push_back(std::move(ld));
	}
}

void ImageWork::pushUndoState() {
	if (Images.empty()) return;
	UndoState state;
	buildUndoState(state);
	undoStack.push_back(std::move(state));
	if ((int)undoStack.size() > MAX_UNDO)
		undoStack.erase(undoStack.begin());
	redoStack.clear(); // new action breaks the redo chain
}

void ImageWork::undo() {
	if (undoStack.empty()) return;

	UndoState cur;
	buildUndoState(cur);
	redoStack.push_back(std::move(cur));

	UndoState prev = std::move(undoStack.back());
	undoStack.pop_back();

	// Restore prev
	for (auto& img : Images) {
		if (img.texture) { glDeleteTextures(1, &img.texture); img.texture = 0; }
		delete[] img.data; img.data = nullptr;
	}
	Images.clear();
	for (const auto& ld : prev.layers) {
		ImageStr img;
		img.name = ld.name;  img.extension = ld.extension;
		img.width = ld.width; img.height = ld.height; img.channels = ld.channels;
		img.visible = ld.visible; img.opacity = ld.opacity;
		img.modified = true; img.loaded = true;
		img.data = new unsigned char[ld.width * ld.height * ld.channels];
		memcpy(img.data, ld.data.data(), ld.width * ld.height * ld.channels);
		glGenTextures(1, &img.texture);
		glBindTexture(GL_TEXTURE_2D, img.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLenum fmt = (img.channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, fmt, img.width, img.height, 0, fmt, GL_UNSIGNED_BYTE, img.data);
		Images.push_back(img);
	}
	{ int cap = Images.empty() ? 0 : (int)Images.size()-1; activeLayerIndex = (prev.activeLayerIndex < cap) ? prev.activeLayerIndex : cap; if (activeLayerIndex < 0) activeLayerIndex = 0; }
	canvasWidth  = prev.canvasWidth;
	canvasHeight = prev.canvasHeight;
	if (!Images.empty()) CurrentImage = Images[activeLayerIndex];
}

void ImageWork::redo() {
	if (redoStack.empty()) return;

	UndoState cur;
	buildUndoState(cur);
	undoStack.push_back(std::move(cur));
	if ((int)undoStack.size() > MAX_UNDO)
		undoStack.erase(undoStack.begin());

	UndoState next = std::move(redoStack.back());
	redoStack.pop_back();

	for (auto& img : Images) {
		if (img.texture) { glDeleteTextures(1, &img.texture); img.texture = 0; }
		delete[] img.data; img.data = nullptr;
	}
	Images.clear();
	for (const auto& ld : next.layers) {
		ImageStr img;
		img.name = ld.name;  img.extension = ld.extension;
		img.width = ld.width; img.height = ld.height; img.channels = ld.channels;
		img.visible = ld.visible; img.opacity = ld.opacity;
		img.modified = true; img.loaded = true;
		img.data = new unsigned char[ld.width * ld.height * ld.channels];
		memcpy(img.data, ld.data.data(), ld.width * ld.height * ld.channels);
		glGenTextures(1, &img.texture);
		glBindTexture(GL_TEXTURE_2D, img.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLenum fmt = (img.channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, fmt, img.width, img.height, 0, fmt, GL_UNSIGNED_BYTE, img.data);
		Images.push_back(img);
	}
	{ int cap = Images.empty() ? 0 : (int)Images.size()-1; activeLayerIndex = (next.activeLayerIndex < cap) ? next.activeLayerIndex : cap; if (activeLayerIndex < 0) activeLayerIndex = 0; }
	canvasWidth  = next.canvasWidth;
	canvasHeight = next.canvasHeight;
	if (!Images.empty()) CurrentImage = Images[activeLayerIndex];
}

bool ImageWork::canUndo() const { return !undoStack.empty(); }
bool ImageWork::canRedo() const { return !redoStack.empty(); }

std::pair<int, int> ImageWork::getInterpolationMin() {
	return this->interpolationMin;
}

std::pair<int, int> ImageWork::getInterpolationMax() {
	return this->interpolationMax;
}

void ImageWork::setInterpolationMin(std::pair<int, int> _interpolationMin) {
	this->interpolationMin = _interpolationMin;
}

void ImageWork::setInterpolationMax(std::pair<int, int> _interpolationMax) {
	this->interpolationMax = _interpolationMax;
}

std::pair<int, int> ImageWork::getSelectionMin() {
	return this->selectionMin;
}

std::pair<int, int> ImageWork::getSelectionMax() {
	return this->selectionMax;
}

void ImageWork::setSelectionMin(std::pair<int, int> _selectionMin) {
	this->selectionMin = _selectionMin;
}

void ImageWork::setSelectionMax(std::pair<int, int> _selectionMax) {
	this->selectionMax = _selectionMax;
}

void ImageWork::clearMousePoints() {
	this->mousePoints.clear();
}

void ImageWork::clearMousePairs() {
	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);
}

void ImageWork::clearFirstPointSelection() {
	this->firstPoint = std::pair<int, int>(-1, -1);
}

void ImageWork::clearSelection() {
	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);
}

void ImageWork::clearImages() {
	for (auto& img : this->Images) {
		if (img.texture) glDeleteTextures(1, &img.texture);
		delete[] img.data;
		img.data = nullptr;
	}
	this->Images.clear();
	activeLayerIndex = 0;
	canvasWidth  = 0;
	canvasHeight = 0;
	undoStack.clear();
	redoStack.clear();

	// Free composite
	if (CompositeImage.texture) {
		glDeleteTextures(1, &CompositeImage.texture);
		CompositeImage.texture = 0;
	}
	delete[] CompositeImage.data;
	CompositeImage.data = nullptr;
	CompositeImage.width = 0;
	CompositeImage.height = 0;
}

void ImageWork::selectionNormalize() {
	if (this->selectionMin.first > this->selectionMax.first) {
		std::swap(this->selectionMin.first, this->selectionMax.first);
	}
	if (this->selectionMin.second > this->selectionMax.second) {
		std::swap(this->selectionMin.second, this->selectionMax.second);
	}

	//If the selection is out of the image, set it to the image size
	if (this->selectionMin.first < 0) {
		this->selectionMin.first = 0;
	}
	if (this->selectionMin.second < 0) {
		this->selectionMin.second = 0;
	}
	if (this->selectionMax.first > this->CurrentImage.width) {
		this->selectionMax.first = this->CurrentImage.width;
	}
	if (this->selectionMax.second > this->CurrentImage.height) {
		this->selectionMax.second = this->CurrentImage.height;
	}
}

void ImageWork::deleteSelection() {
	//if the selection is done and the selection is enabled the set the selected pixels to the secondary color
	if (this->selectionDone && this->selectionEnable) {
		for (int i = this->selectionMin.first; i < this->selectionMax.first; i++) {
			for (int j = this->selectionMin.second; j < this->selectionMax.second; j++) {
				this->setPixel(i, j, this->secondaryColor);
			}
		}
	}
}

void ImageWork::PushNewImage(ImageStr _Image) {
	this->Images.push_back(_Image);
}

int ImageWork::ImagesSize() {
	return (int)this->Images.size();
}

ImageStr* ImageWork::getImage(int _index) {
	return &this->Images.at(_index);
}

int ImageWork::getSizeImages() {
	return (int)this->Images.size();
}

void ImageWork::swapImages(int _indexa, int _indexb) {
	std::swap(this->Images.at(_indexa), this->Images.at(_indexb));
}

ImageStr* ImageWork::getImageStrP() {
	if (!Images.empty() && activeLayerIndex >= 0 && activeLayerIndex < (int)Images.size())
		return &Images[activeLayerIndex];
	return &CurrentImage;
}

bool ImageWork::useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY) {
	// Reset dirty rect each call so UpdateImage() gets a per-stroke-segment region
	resetInterpolationBounds();

	switch (_tool)
	{
	case TOOL_PENCIL:
		toolPencil(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_BRUSH:
		toolBrush(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_ERASER:
		toolEraser(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_COLORPICKER:
		toolColorPicker(_MouseImagePositionX, _MouseImagePositionY);
		break;
	case TOOL_SELECTION:
		toolSelection(_MouseImagePositionX, _MouseImagePositionY);
		break;
	case TOOL_BUCKET:
		toolBucket(_MouseImagePositionX, _MouseImagePositionY, this->Tolerance);
		break;
	case TOOL_TEXT:
		toolText(_MouseImagePositionX, _MouseImagePositionY);
		break;

	default:
		this->setSelectionEnable(false);
		break;
	}

	return true;
}

void ImageWork::setToolColor(RGB _color) {
	this->toolColor = _color;
}

void ImageWork::setSecondaryColor(RGB _color) {
	this->secondaryColor = _color;
}

void ImageWork::setToolRadius(int _radius) {
	this->toolradius = _radius;
}

void ImageWork::setSelectionEnable(bool _enable) {
	this->selectionEnable = _enable;
}

void ImageWork::setSelectionDone(bool _done) {
	this->selectionDone = _done;
}

void ImageWork::setTolerance(int _tolerance) {
	this->Tolerance = _tolerance;
}

RGB ImageWork::getToolColor() {
	return this->toolColor;
}

RGB ImageWork::getSecondaryColor() {
	return this->secondaryColor;
}

int ImageWork::getToolRadius() {
	return this->toolradius;
}

bool ImageWork::getSelectionEnabled() {
	return this->selectionEnable;
}

bool ImageWork::getSelectionDone() {
	return this->selectionDone;
}

int ImageWork::getTolerance() {
	return this->Tolerance;
}

bool ImageWork::getTextEnabled() {
	return this->textEnable;
}

void ImageWork::setTextEnabled(bool _enable) {
	this->textEnable = _enable;
}

std::pair<int, int> ImageWork::getTextPosition() {
	return this->textPosition;
}

void ImageWork::setTextPosition(std::pair<int, int> _textPosition) {
	this->textPosition = _textPosition;
}

std::string ImageWork::getText() {
	return this->text;
}

void ImageWork::setText(std::string _text) {
	this->text = _text;
}

RGB ImageWork::getPixel(int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);
	ImageStr* target = getImageStrP();
	if (!target || target->data == nullptr) return pixel;

	if (_x >= 0 && _x < target->width && _y >= 0 && _y < target->height) {
		int base = (_y * target->width + _x) * target->channels;
		pixel.r = target->data[base    ];
		pixel.g = target->data[base + 1];
		pixel.b = target->data[base + 2];
		pixel.delta = (target->channels == 4) ? target->data[base + 3] : 255;
	}

	return pixel;
}

RGB ImageWork::getPixel(ImageStr* _image, int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);

	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		pixel.r = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 0];
		pixel.g = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 1];
		pixel.b = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 2];
		if (_image->channels == 4) {
			pixel.delta = _image->data[_y * _image->width * 4 + _x * 4 + 3];
		}
	}

	return pixel;
}

void ImageWork::setPixel(int _x, int _y, RGB _color) {
	ImageStr* target = getImageStrP();
	if (!target || target->data == nullptr) return;

	if (_x >= 0 && _x < target->width && _y >= 0 && _y < target->height) {
		int index = (_y * target->width + _x) * target->channels;
		target->data[index    ] = _color.r;
		target->data[index + 1] = _color.g;
		target->data[index + 2] = _color.b;
		if (target->channels == 4)
			target->data[index + 3] = _color.delta;
		target->modified = true;
	}
}

void ImageWork::setPixel(ImageStr* _image, int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		int index = (_y * _image->width + _x) * _image->channels;
		_image->data[index] = _color.r;
		_image->data[index + 1] = _color.g;
		_image->data[index + 2] = _color.b;
		if (_image->channels == 4) {
			_image->data[index + 3] = _color.delta;
		}
		_image->modified = true;
	}
}

void ImageWork::reCopyImage(ImageStr* _Image) {
	this->Images.front() = *_Image;
}

void ImageWork::blackAndWhite(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			unsigned char average = (aux.r + aux.g + aux.b) / 3;
			setPixel(_Image, i, j, RGB(average,average,average,aux.delta));
		}
	}
}

void ImageWork::negative(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			setPixel(_Image, i, j, RGB(255 - aux.r, 255 - aux.g, 255 - aux.b, aux.delta));
		}
	}
}

void ImageWork::sepia(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			unsigned char average = (aux.r + aux.g + aux.b) / 3;
			setPixel(_Image, i, j, RGB((unsigned char)(average * 0.9), (unsigned char)(average * 0.7), (unsigned char)(average * 0.5), aux.delta));
		}
	}
}
