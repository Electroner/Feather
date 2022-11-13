#include "../feathergui.h"

bool FeatherGUI::loadImage(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading image: " << _path << std::endl;
	//Get the numbers of channels in the image
	int channels;
	unsigned char* image = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels, 0);
	if (!image) {
		std::cout << "Error Loading the Image: " << _path << std::endl;
		return false;
	}
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load image '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//free image
	stbi_image_free(image);

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
	if (channels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	stbi_image_free(unloadedImage.data);

	//Reserve memory for the image
	unloadedImage.data = new GLubyte[unloadedImage.width * unloadedImage.height * unloadedImage.channels];

	if (channels == 4)
	{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else
	{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}

	unloadedImage.loaded = true;

	//Add image to Images vector
	this->Images->push_back(unloadedImage);

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
	if (channels == 4) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	//Add image to Images vector
	this->Images->push_back(unloadedImage);

	//free bitmap
	DeleteObject(hBitmap);
	
	//Set the image as selected
	this->workStation.selectFrontImage();
	//Set the zooom
	this->calculateZoom();
	this->centerImage();

	//Update the image
	this->UpdateImage();

	return true;
}

bool FeatherGUI::saveImage(std::string _path) {
	if (_path.size() == 0)
	{
		std::cout << "Error saving image: No path specified" << std::endl;
		return false;
	}
	std::string location;
	//_path includes the name of the image
	location = _path + "." + this->CurrentImage->extension;
	std::cout << "Image saved at: " << location << std::endl;
	stbi_write_png(location.c_str(), this->CurrentImage->width, this->CurrentImage->height, this->CurrentImage->channels, this->CurrentImage->data, this->CurrentImage->width * this->CurrentImage->channels);
	return true;
}

bool FeatherGUI::copySelectionToClipboard() {
	//Get the selection
	std::pair<int,int> selectionMin = this->workStation.getSelectionMin();
	std::pair<int,int> selectionMax = this->workStation.getSelectionMax();
	GLubyte* data = new GLubyte[(selectionMax.first - selectionMin.first) * (selectionMax.second - selectionMin.second) * 4];

	//Copy the selection to the data array interchanging the red and blue channels (BGRA) 
	for (int i = 0; i < (selectionMax.first - selectionMin.first); i++) {
		for (int j = 0; j < (selectionMax.second - selectionMin.second); j++) {
			//Assume 4 channels
			//If the the image has 3 channels the alpha channel will be 255 and we need to skip 3 by 3
			if (this->CurrentImage->channels == 4) {
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 4 + 2];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 1] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 4 + 1];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 2] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 4];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 3] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 4 + 3];
			}
			else 
			{
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 3 + 2];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 1] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 3 + 1];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 2] = this->CurrentImage->data[(i + selectionMin.first + (j + selectionMin.second) * this->CurrentImage->width) * 3];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 3] = 255;
			}
		}
	}
	
	//Create bitmap
	HBITMAP hBitmap = CreateBitmap(selectionMax.first - selectionMin.first, selectionMax.second - selectionMin.second, 1, 32, data);
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
	//Copy the image to the data array interchanging the red and blue channels (BGRA)
	GLubyte* data = new GLubyte[this->CurrentImage->width * this->CurrentImage->height * 4];
	
	for (int i = 0; i < this->CurrentImage->width; i++) {
		for (int j = 0; j < this->CurrentImage->height; j++) {
			//Assume 4 channels
			//If the the image has 3 channels the alpha channel will be 255 and we need to skip 3 by 3
			if (this->CurrentImage->channels == 4) {
				data[(i + j * this->CurrentImage->width) * 4] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 4 + 2];
				data[(i + j * this->CurrentImage->width) * 4 + 1] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 4 + 1];
				data[(i + j * this->CurrentImage->width) * 4 + 2] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 4];
				data[(i + j * this->CurrentImage->width) * 4 + 3] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 4 + 3];
			}
			else
			{
				data[(i + j * this->CurrentImage->width) * 4] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 3 + 2];
				data[(i + j * this->CurrentImage->width) * 4 + 1] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 3 + 1];
				data[(i + j * this->CurrentImage->width) * 4 + 2] = this->CurrentImage->data[(i + j * this->CurrentImage->width) * 3];
				data[(i + j * this->CurrentImage->width) * 4 + 3] = 255;
			}
		}
	}
	
	//Create bitmap
	HBITMAP hBitmap = CreateBitmap(this->CurrentImage->width, this->CurrentImage->height, 1, 32, data);
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