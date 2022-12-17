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

	//Add image to Images vector
	this->workStation.PushNewImage(unloadedImage);

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

	//Add image to Images vector
	this->workStation.PushNewImage(unloadedImage);

	//free bitmap
	DeleteObject(hBitmap);
	
	//Set the image as selected
	this->workStation.combineLayers();
	//Set the zooom
	this->calculateZoom();
	this->centerImage();

	//Update the image
	this->UpdateImage();

	return true;
}

bool FeatherGUI::saveImage(std::string _path) {
	std::string location;
	if (this->workStation.getSelectionEnabled()) {
		//Save only the selected region
		if (_path.size() == 0)
		{
			std::cout << "Error saving image: No path specified" << std::endl;
			return false;
		}
		//Min and max coordinates of the selection
		std::pair<int, int> selectionMin = this->workStation.getSelectionMin();
		std::pair<int, int> selectionMax = this->workStation.getSelectionMax();
		GLubyte* SelectedData = new GLubyte[(selectionMax.first - selectionMin.first) * (selectionMax.second - selectionMin.second) * this->workStation.getImageStrP()->channels];

		//Copy the selection to the data array interchanging the red and blue channels (BGRA) 
		for (int i = 0; i < (selectionMax.first - selectionMin.first); i++) {
			for (int j = 0; j < (selectionMax.second - selectionMin.second); j++) {
				SelectedData[(i + j * (selectionMax.first - selectionMin.first)) * 4] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4];
				SelectedData[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 1] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 1];
				SelectedData[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 2] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 2];
				SelectedData[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 3] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 3];
			}
		}

		//_path includes the name of the image
		location = _path + "." + this->workStation.getImageStrP()->extension;
		std::cout << "Image saved at: " << location << std::endl;
		//Save the image
		stbi_write_png(location.c_str(), selectionMax.first - selectionMin.first, selectionMax.second - selectionMin.second, this->workStation.getImageStrP()->channels, SelectedData, (selectionMax.first - selectionMin.first) * this->workStation.getImageStrP()->channels);
		
		//Free memory
		delete[] SelectedData;
		return true;
	}
	else 
	{
		//Save the entire image
		if (_path.size() == 0)
		{
			std::cout << "Error saving image: No path specified" << std::endl;
			return false;
		}
		//_path includes the name of the image
		location = _path + "." + this->workStation.getImageStrP()->extension;
		std::cout << "Image saved at: " << location << std::endl;
		stbi_write_png(location.c_str(), this->workStation.getImageStrP()->width,
			this->workStation.getImageStrP()->height,
			this->workStation.getImageStrP()->channels,
			this->workStation.getImageStrP()->data, 
			this->workStation.getImageStrP()->width * this->workStation.getImageStrP()->channels);
		return true;
	}
	return false;
}

bool FeatherGUI::copySelectionToClipboard() {
	//Get the selection
	std::pair<int,int> selectionMin = this->workStation.getSelectionMin();
	std::pair<int,int> selectionMax = this->workStation.getSelectionMax();
	
	//Check if the selection is valid, the selection has to be at least 1 pixel wide and 1 pixel high and the selection has to be inside the image
	if (selectionMin.first == selectionMax.first || selectionMin.second == selectionMax.second || selectionMin.first < 0 || selectionMin.second < 0 || selectionMax.first > this->workStation.getImageStrP()->width || selectionMax.second > this->workStation.getImageStrP()->height) {
		std::cout << "Error copying to clipboard: Invalid selection" << std::endl;
		return false;
	}

	GLubyte* data = new GLubyte[(selectionMax.first - selectionMin.first) * (selectionMax.second - selectionMin.second) * 4];

	//Copy the selection to the data array interchanging the red and blue channels (BGRA) 
	for (int i = 0; i < (selectionMax.first - selectionMin.first); i++) {
		for (int j = 0; j < (selectionMax.second - selectionMin.second); j++) {
			//Assume 4 channels
			//If the the image has 3 channels the alpha channel will be 255 and we need to skip 3 by 3
			if (this->workStation.getImageStrP()->channels == 4) {
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 2];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 1] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 1];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 2] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 3] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 4 + 3];
			}
			else 
			{
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 3 + 2];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 1] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 3 + 1];
				data[(i + j * (selectionMax.first - selectionMin.first)) * 4 + 2] = this->workStation.getImageStrP()->data[(i + selectionMin.first + (j + selectionMin.second) * this->workStation.getImageStrP()->width) * 3];
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
	//Check if the image is valid
	if (this->workStation.getImageStrP()->data == NULL) {
		std::cout << "Error copying to clipboard: No image loaded" << std::endl;
		return false;
	}
	if (this->workStation.getImageStrP()->width > 0 && this->workStation.getImageStrP()->height > 0) {
		//Copy the image to the data array interchanging the red and blue channels (BGRA)
		GLubyte* data = new GLubyte[this->workStation.getImageStrP()->width * this->workStation.getImageStrP()->height * 4];

		for (int i = 0; i < this->workStation.getImageStrP()->width; i++) {
			for (int j = 0; j < this->workStation.getImageStrP()->height; j++) {
				//Assume 4 channels
				//If the the image has 3 channels the alpha channel will be 255 and we need to skip 3 by 3
				if (this->workStation.getImageStrP()->channels == 4) {
					data[(i + j * this->workStation.getImageStrP()->width) * 4] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 4 + 2];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 1] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 4 + 1];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 2] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 4];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 3] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 4 + 3];
				}
				else
				{
					data[(i + j * this->workStation.getImageStrP()->width) * 4] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 3 + 2];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 1] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 3 + 1];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 2] = this->workStation.getImageStrP()->data[(i + j * this->workStation.getImageStrP()->width) * 3];
					data[(i + j * this->workStation.getImageStrP()->width) * 4 + 3] = 255;
				}
			}
		}

		//Create bitmap
		HBITMAP hBitmap = CreateBitmap(this->workStation.getImageStrP()->width, this->workStation.getImageStrP()->height, 1, 32, data);
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
	}
	
	return true;
}