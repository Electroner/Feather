#include "../ImageWork.h"

void ImageWork::calculateHistogram(ImageStr* _Image) {
	//Make sure the image is not null and the histogram is big enough
	if (_Image != nullptr) {
		//Make sure the image is not empty
		if (_Image->width > 0 && _Image->height > 0) {
			//Reserve the memory for the histogram
			_Image->histogramR = new int[256];
			_Image->histogramG = new int[256];
			_Image->histogramB = new int[256];

			//Calculate the histogram
			for (int i = 0; i < 256; i++) {
				_Image->histogramR[i] = 0;
				_Image->histogramG[i] = 0;
				_Image->histogramB[i] = 0;
			}

			for (int i = 0; i < _Image->width * _Image->height; i++) {
				_Image->histogramR[_Image->data[i * _Image->channels + 0]]++;
				_Image->histogramG[_Image->data[i * _Image->channels + 1]]++;
				_Image->histogramB[_Image->data[i * _Image->channels + 2]]++;
			}

			//Search for the maximum value
			for (int i = 0; i < 256; i++) {
				//histogramRSize
				if (_Image->histogramR[i] > _Image->histogramRSize) {
					_Image->histogramRSize = _Image->histogramR[i];
				}
				//histogramGSize
				if (_Image->histogramG[i] > _Image->histogramGSize) {
					_Image->histogramGSize = _Image->histogramG[i];
				}
				//histogramBSize
				if (_Image->histogramB[i] > _Image->histogramBSize) {
					_Image->histogramBSize = _Image->histogramB[i];
				}
			}
		}
	}
}