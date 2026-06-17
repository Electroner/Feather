#include "../ImageWork.h"

void ImageWork::resizeImage(ImageStr* _image, int _width, int _height) {
	if (!_image || _image->width <= 0 || _image->height <= 0) return;
	if (_image->width == _width && _image->height == _height) return;
	if (_width <= 0 || _height <= 0) return;

	GLubyte* newData = new GLubyte[_width * _height * 4];

	// Bilinear interpolation
	float scaleX = (float)_image->width  / (float)_width;
	float scaleY = (float)_image->height / (float)_height;

	for (int i = 0; i < _height; i++) {
		float fy = i * scaleY;
		int   y1 = (int)fy;
		int   y2 = y1 + 1;
		if (y2 >= _image->height) y2 = _image->height - 1;
		float b = fy - y1;

		for (int j = 0; j < _width; j++) {
			float fx = j * scaleX;
			int   x1 = (int)fx;
			int   x2 = x1 + 1;
			if (x2 >= _image->width) x2 = _image->width - 1;
			float a = fx - x1;

			// Sample 4 neighbours (already 0-255 uchar values)
			RGB p00 = getPixel(_image, x1, y1);
			RGB p10 = getPixel(_image, x2, y1);
			RGB p01 = getPixel(_image, x1, y2);
			RGB p11 = getPixel(_image, x2, y2);

			float fa = a, fb = b;
			float w00 = (1.0f - fa) * (1.0f - fb);
			float w10 = fa          * (1.0f - fb);
			float w01 = (1.0f - fa) * fb;
			float w11 = fa          * fb;

			int idx = (i * _width + j) * 4;
			newData[idx    ] = (GLubyte)(p00.r     * w00 + p10.r     * w10 + p01.r     * w01 + p11.r     * w11);
			newData[idx + 1] = (GLubyte)(p00.g     * w00 + p10.g     * w10 + p01.g     * w01 + p11.g     * w11);
			newData[idx + 2] = (GLubyte)(p00.b     * w00 + p10.b     * w10 + p01.b     * w01 + p11.b     * w11);
			newData[idx + 3] = (GLubyte)(p00.delta * w00 + p10.delta * w10 + p01.delta * w01 + p11.delta * w11);
		}
	}

	// Update texture
	glBindTexture(GL_TEXTURE_2D, _image->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newData);

	delete[] _image->data;
	_image->data     = newData;
	_image->width    = _width;
	_image->height   = _height;
	_image->channels = 4;
	_image->modified = true;

	this->combineLayers();
}
