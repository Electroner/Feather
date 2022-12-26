#include "../ImageWork.h"

void ImageWork::resizeImage(ImageStr* _image, int _width, int _height) {
	//Check if the image is not null
	if (_image != nullptr) {
		//Check if the image is not empty
		if (_image->width > 0 && _image->height > 0) {
			//Check if the image is not the same size
			if (_image->width != _width || _image->height != _height) {
				//Resize the image
				GLubyte* newData = new GLubyte[_width * _height * 4];

				//Bilinear interpolation for resampling the new image
				for (int i = 0; i < _height; i++) {
					for (int j = 0; j < _width; j++) {
						float x = (float)j / (float)_width * (float)_image->width;
						float y = (float)i / (float)_height * (float)_image->height;

						int x1 = (int)floor(x);
						int x2 = (int)ceil(x);
						int y1 = (int)floor(y);
						int y2 = (int)ceil(y);

						float a = x - x1;
						float b = y - y1;

						//Get the 4 pixels
						RGB p1 = getPixel(_image, x1, y1);
						RGB p2 = getPixel(_image, x2, y1);
						RGB p3 = getPixel(_image, x1, y2);
						RGB p4 = getPixel(_image, x2, y2);

						//Interpolate the 4 pixels
						RGB p12;
						p12.r = (unsigned char)(((1 - a) * p1.r + a * p2.r) * 255);
						p12.g = (unsigned char)(((1 - a) * p1.g + a * p2.g) * 255);
						p12.b = (unsigned char)(((1 - a) * p1.b + a * p2.b) * 255);
						p12.delta = (unsigned char)(((1 - a) * p1.delta + a * p2.delta) * 255);

						RGB p34;
						p34.r = (unsigned char)(((1 - a) * p3.r + a * p4.r) * 255);
						p34.g = (unsigned char)(((1 - a) * p3.g + a * p4.g) * 255);
						p34.b = (unsigned char)(((1 - a) * p3.b + a * p4.b) * 255);
						p34.delta = (unsigned char)(((1 - a) * p3.delta + a * p4.delta) * 255);

						//Interpolate the 2 pixels
						RGB p1234;
						p1234.r = (unsigned char)(((1 - b) * p12.r + b * p34.r) * 255);
						p1234.g = (unsigned char)(((1 - b) * p12.g + b * p34.g) * 255);
						p1234.b = (unsigned char)(((1 - b) * p12.b + b * p34.b) * 255);
						p1234.delta = (unsigned char)(((1 - b) * p12.delta + b * p34.delta) * 255);

						//Set the new pixel
						newData[i * _width * 4 + j * 4 + 0] = p1234.r;
						newData[i * _width * 4 + j * 4 + 1] = p1234.g;
						newData[i * _width * 4 + j * 4 + 2] = p1234.b;
						newData[i * _width * 4 + j * 4 + 3] = p1234.delta;
					}
				}

				//Delete the old data
				delete[] _image->data;

				//Set the new data
				_image->data = newData;

				//Set the new width and height
				_image->width = _width;
				_image->height = _height;

				this->combineLayers();
			}
		}
	}
}