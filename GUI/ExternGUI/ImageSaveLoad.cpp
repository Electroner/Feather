#include "../feathergui.h"

bool FeatherGUI::loadImage(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading image: " << _path << std::endl;
	
	//Get the numbers of channels in the image file with stb_info
	int channels;
	int error = stbi_info(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels);

	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	
	//If the image has different channels than 4 then we need to convert it to 4 channels
	unsigned char* temp;
	switch (unloadedImage.channels)
	{
		case 1:
			unloadedImage.channels = 4;
			temp = new unsigned char[unloadedImage.width * unloadedImage.height * unloadedImage.channels];
			for (int i = 0; i < unloadedImage.width * unloadedImage.height; i++) {
				temp[i * 4] = unloadedImage.data[i];
				temp[i * 4 + 1] = unloadedImage.data[i];
				temp[i * 4 + 2] = unloadedImage.data[i];
				temp[i * 4 + 3] = 255;
			}
			stbi_image_free(unloadedImage.data);
			unloadedImage.data = temp;
			break;
		case 2:
			unloadedImage.channels = 4;
			temp = new unsigned char[unloadedImage.width * unloadedImage.height * unloadedImage.channels];
			for (int i = 0; i < unloadedImage.width * unloadedImage.height; i++) {
				temp[i * 4] = unloadedImage.data[i * 2];
				temp[i * 4 + 1] = unloadedImage.data[i * 2];
				temp[i * 4 + 2] = unloadedImage.data[i * 2];
				temp[i * 4 + 3] = unloadedImage.data[i * 2 + 1];
			}
			stbi_image_free(unloadedImage.data);
			unloadedImage.data = temp;
			break;
		case 3:
			unloadedImage.channels = 4;
			temp = new unsigned char[unloadedImage.width * unloadedImage.height * unloadedImage.channels];
			for (int i = 0; i < unloadedImage.width * unloadedImage.height; i++) {
				temp[i * 4] = unloadedImage.data[i * 3];
				temp[i * 4 + 1] = unloadedImage.data[i * 3 + 1];
				temp[i * 4 + 2] = unloadedImage.data[i * 3 + 2];
				temp[i * 4 + 3] = 255;
			}
			stbi_image_free(unloadedImage.data);
			unloadedImage.data = temp;
			break;
		default:
			break;
	}
	
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load image '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Image extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Image name: " << unloadedImage.name << std::endl << std::endl;

	//Enable transparency
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create a OpenGL texture identifier and binding
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);

	unloadedImage.loaded = true;
	unloadedImage.modified = false;

	// Add as the topmost layer and make it active
	this->workStation.insertImageOnTop(unloadedImage);

	return true;
}

bool FeatherGUI::loadIcon(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading icon: " << _path << std::endl;
	int channels;
	unsigned char* image = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels, 0);
	if (!image) {
		std::cout << "Error Loading the icon: " << _path << std::endl;
		return false;
	}
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load icon '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//free image
	stbi_image_free(image);

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Icon extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Icon name: " << unloadedImage.name << std::endl << std::endl;

	// Create a OpenGL texture identifier and binding
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	stbi_image_free(unloadedImage.data);

	unloadedImage.loaded = true;

	//Add image to Images vector
	toolsIcons.push_back(unloadedImage);

	return true;
}

bool::FeatherGUI::loadCursor(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading icon: " << _path << std::endl;
	int channels;
	unsigned char* image = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels, 0);
	if (!image) {
		std::cout << "Error Loading the icon: " << _path << std::endl;
		return false;
	}
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load icon '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//free image
	stbi_image_free(image);

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Icon extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Icon name: " << unloadedImage.name << std::endl << std::endl;

	// Create a OpenGL texture identifier and binding
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	stbi_image_free(unloadedImage.data);

	unloadedImage.loaded = true;

	//Add image to Images vector
	cursors.push_back(unloadedImage);

	return true;
}

bool FeatherGUI::loadFromClipBoard() {
	std::cout << "Loading from clipboard" << std::endl;
	//Show content of clipboard with windows api
	//Open clipboard
	if (!OpenClipboard(NULL)) {
		std::cout << "Error opening clipboard" << std::endl;
		CloseClipboard();
		return false;
	}
	//Get clipboard data as image format CBitmap 
	HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (hBitmap == NULL) {
		std::cout << "Error getting clipboard data" << std::endl;
		CloseClipboard();
		return false;
	}
	//Close clipboard
	CloseClipboard();
	
	//Get bitmap info
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	
	int channels = 4;
	
	//Get bitmap data
	GLubyte* data = new GLubyte[bitmap.bmWidth * bitmap.bmHeight * channels];
	GetBitmapBits(hBitmap, bitmap.bmWidth * bitmap.bmHeight * channels, data);

	//Incherchange the red and blue channels (BGRA)
	for (int i = 0; i < bitmap.bmWidth * bitmap.bmHeight * channels; i += channels) {
		GLubyte temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}

	//Create image
	ImageStr unloadedImage;
	unloadedImage.data = data;
	unloadedImage.width = bitmap.bmWidth;
	unloadedImage.height = bitmap.bmHeight;
	unloadedImage.channels = channels;
	unloadedImage.name = "Clipboard";
	unloadedImage.extension = "png";
	unloadedImage.imagePath = ".";
	unloadedImage.modified = false;
	unloadedImage.loaded = true;
	//Create texture
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);
	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);

	// Add as topmost layer
	this->workStation.insertImageOnTop(unloadedImage);

	DeleteObject(hBitmap);

	this->workStation.combineLayers();
	this->calculateZoom();
	this->centerImage();
	this->UpdateImage();

	return true;
}

bool FeatherGUI::saveImage(std::string _path) {
	if (_path.empty()) {
		std::cout << "Error saving image: No path specified" << std::endl;
		return false;
	}

	// Always save the composited (flattened) view
	this->workStation.compositeAllLayers();
	ImageStr* img = this->workStation.getCompositeImage();

	if (!img || !img->data) {
		std::cout << "Error saving image: No composite available" << std::endl;
		return false;
	}

	std::string ext = this->workStation.getImageStrP()->extension;
	if (ext.empty()) ext = "png";

	if (this->workStation.getSelectionEnabled()) {
		// Save only the selected region from the composite
		std::pair<int, int> selMin = this->workStation.getSelectionMin();
		std::pair<int, int> selMax = this->workStation.getSelectionMax();
		int selW = selMax.first  - selMin.first;
		int selH = selMax.second - selMin.second;
		if (selW <= 0 || selH <= 0) return false;

		GLubyte* selData = new GLubyte[selW * selH * 4];
		for (int j = 0; j < selH; j++) {
			for (int i = 0; i < selW; i++) {
				int src = ((j + selMin.second) * img->width + (i + selMin.first)) * 4;
				int dst = (j * selW + i) * 4;
				selData[dst    ] = img->data[src    ];
				selData[dst + 1] = img->data[src + 1];
				selData[dst + 2] = img->data[src + 2];
				selData[dst + 3] = img->data[src + 3];
			}
		}
		std::string location = _path + "." + ext;
		std::cout << "Selection saved at: " << location << std::endl;
		stbi_write_png(location.c_str(), selW, selH, 4, selData, selW * 4);
		delete[] selData;
		return true;
	} else {
		std::string location = _path + "." + ext;
		std::cout << "Image saved at: " << location << std::endl;
		stbi_write_png(location.c_str(), img->width, img->height, 4, img->data, img->width * 4);
		return true;
	}
}

bool FeatherGUI::copySelectionToClipboard() {
	std::pair<int,int> selectionMin = this->workStation.getSelectionMin();
	std::pair<int,int> selectionMax = this->workStation.getSelectionMax();

	// Use composite so all visible layers are included
	this->workStation.compositeAllLayers();
	ImageStr* src = this->workStation.getCompositeImage();

	if (!src || !src->data ||
		selectionMin.first == selectionMax.first || selectionMin.second == selectionMax.second ||
		selectionMin.first < 0 || selectionMin.second < 0 ||
		selectionMax.first > src->width || selectionMax.second > src->height) {
		std::cout << "Error copying to clipboard: Invalid selection" << std::endl;
		return false;
	}

	int selW = selectionMax.first  - selectionMin.first;
	int selH = selectionMax.second - selectionMin.second;
	GLubyte* data = new GLubyte[selW * selH * 4];

	// Copy RGBA→BGRA for Windows clipboard
	for (int j = 0; j < selH; j++) {
		for (int i = 0; i < selW; i++) {
			int srcIdx = ((j + selectionMin.second) * src->width + (i + selectionMin.first)) * 4;
			int dstIdx = (j * selW + i) * 4;
			data[dstIdx    ] = src->data[srcIdx + 2]; // B
			data[dstIdx + 1] = src->data[srcIdx + 1]; // G
			data[dstIdx + 2] = src->data[srcIdx    ]; // R
			data[dstIdx + 3] = src->data[srcIdx + 3]; // A
		}
	}
	
	HBITMAP hBitmap = CreateBitmap(selW, selH, 1, 32, data);
	if (hBitmap == NULL) {
		std::cout << "Error creating bitmap" << std::endl;
		return false;
	}
	
	//Open clipboard
	if (!OpenClipboard(NULL)) {
		std::cout << "Error opening clipboard" << std::endl;
		CloseClipboard();
		return false;
	}
	//Empty clipboard
	EmptyClipboard();
	//Set clipboard data
	if (!SetClipboardData(CF_BITMAP, hBitmap)) {
		std::cout << "Error setting clipboard data" << std::endl;
		CloseClipboard();
		return false;
	}
	//Close clipboard
	CloseClipboard();
	
	//Free bitmap
	DeleteObject(hBitmap);

	//free data
	delete[] data;
	
	return true;
}

bool FeatherGUI::copyImageToClipboard() {
	// Use composite so all visible layers are copied
	this->workStation.compositeAllLayers();
	ImageStr* src = this->workStation.getCompositeImage();

	if (!src || !src->data || src->width <= 0 || src->height <= 0) {
		std::cout << "Error copying to clipboard: No image loaded" << std::endl;
		return false;
	}

	int w = src->width;
	int h = src->height;
	GLubyte* data = new GLubyte[w * h * 4];

	// Convert RGBA → BGRA for Windows clipboard
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			int idx = (j * w + i) * 4;
			data[idx    ] = src->data[idx + 2]; // B
			data[idx + 1] = src->data[idx + 1]; // G
			data[idx + 2] = src->data[idx    ]; // R
			data[idx + 3] = src->data[idx + 3]; // A
		}
	}

	HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
	if (hBitmap == NULL) {
		std::cout << "Error creating bitmap" << std::endl;
		delete[] data;
		return false;
	}

	if (!OpenClipboard(NULL)) {
		std::cout << "Error opening clipboard" << std::endl;
		DeleteObject(hBitmap);
		delete[] data;
		return false;
	}
	EmptyClipboard();
	if (!SetClipboardData(CF_BITMAP, hBitmap)) {
		std::cout << "Error setting clipboard data" << std::endl;
		CloseClipboard();
		DeleteObject(hBitmap);
		delete[] data;
		return false;
	}
	CloseClipboard();
	DeleteObject(hBitmap);
	delete[] data;
	
	return true;
}

void FeatherGUI::drop_callback(GLFWwindow* _windowContext, int _count, const char** _paths) {
	for (int i = 0; i < _count; i++){
		droppedFiles.push_back(_paths[i]);
	}
}

bool FeatherGUI::loadDroppedImages() {
	bool done = true;
	
	for(int i = 0; i < droppedFiles.size(); i++) {
		done &= this->loadImage(getNormalizedPath(droppedFiles[i]));
	}

	droppedFiles.clear();

	workStation.combineLayers();
	this->centerImage();
	this->UpdateImage();

	return done;
}