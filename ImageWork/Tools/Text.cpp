#include "../ImageWork.h"

ImageStr rasterText(const char* text, const char* fontPath, int size, int spaceSizeBtLetters, int spaceSize, const RGB& color)
{
	int totalWidth = 0;
	int totalHeight = 0;
	std::vector<ImageStr> letters;
	ImageStr word;
	
	bool correct = true;
	
	// Initialize FreeType library
	FT_Library library;
	if (FT_Init_FreeType(&library))
	{
		// Could not initialize FreeType library
		std::cout << "FreeType library cannot initialice" << std::endl;
		correct = false;
	}
	// Load font face from file
	FT_Face face;
	if (FT_New_Face(library, fontPath, 0, &face))
	{
		// Could not load font face
		std::cout << "Font face not loaded" << std::endl;
		correct = false;
	}
	
	if (correct) {
		// Set font size
		FT_Set_Pixel_Sizes(face, 0, size);

		//Iterate through the text
		for (int i = 0; i < strlen(text); i++)
		{
			if (text[i] == ' ') 
			{
				// Create image
				ImageStr letter;
				letter.width = spaceSize;
				letter.height = 0;
				letter.channels = 1;
				letter.data = new GLubyte[letter.width];
				letter.texture = -1;
				letter.histogramR = NULL;
				letter.histogramRSize = 0;
				letter.histogramG = NULL;
				letter.histogramGSize = 0;
				letter.histogramB = NULL;
				letter.histogramBSize = 0;
				letter.modified = false;
				letter.loaded = false;
				letter.name = text[i];

				// Add letter to vector
				letters.push_back(letter);
				totalWidth += spaceSize;
			}
			else
			{
				// Load glyph
				if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))
				{
					// Could not load glyph
					continue;
				}
		
				// Create image
				ImageStr letter;
				letter.width = face->glyph->bitmap.width;
				letter.height = face->glyph->bitmap.rows;
				letter.channels = 1;
				letter.data = new GLubyte[letter.width * letter.height * letter.channels];
				letter.texture = -1;
				letter.histogramR = NULL;
				letter.histogramRSize = 0;
				letter.histogramG = NULL;
				letter.histogramGSize = 0;
				letter.histogramB = NULL;
				letter.histogramBSize = 0;
				letter.modified = false;
				letter.loaded = false;
				letter.name = text[i];
		
				// Copy glyph bitmap to image
				for (int y = 0; y < letter.height; y++)
				{
					for (int x = 0; x < letter.width; x++)
					{
						letter.data[y * letter.width + x] = face->glyph->bitmap.buffer[y * letter.width + x];
					}
				}
				// Add image to vector
				letters.push_back(letter);
				// Add width to total width
				totalWidth += letter.width;
				// Add height to total height if it is greater than the current total height
				if (letter.height > totalHeight)
				{
					totalHeight = letter.height;
				}
			}
		}

		// Create image for the word
		word.width = totalWidth + ((int)letters.size() - 1) * spaceSizeBtLetters;
		word.height = totalHeight;
		word.channels = 4;
		word.data = new GLubyte[word.width * word.height * word.channels];
		word.texture = -1;
		word.histogramR = NULL;
		word.histogramRSize = 0;
		word.histogramG = NULL;
		word.histogramGSize = 0;
		word.histogramB = NULL;
		word.histogramBSize = 0;
		word.modified = false;
		word.loaded = true;
		word.name = text;
		word.extension = "png";
	
		//clear data
		for (int i = 0; i < word.width * word.height * word.channels; i++)
		{
			word.data[i] = 0;
		}

		//Copy letters to word
		//The letters are copied from the bottom left corner
		int x = 0;
		int y = 0;
		for (int i = 0; i < letters.size(); i++)
		{
			y = word.height - letters[i].height;
			//Copy letter
			for (int j = 0; j < letters[i].height; j++)
			{
				for (int k = 0; k < letters[i].width; k++)
				{
					//Copy pixel
					word.data[(y + j) * word.width * word.channels + (x + k) * word.channels] = letters[i].data[j * letters[i].width + k];
					word.data[(y + j) * word.width * word.channels + (x + k) * word.channels + 1] = letters[i].data[j * letters[i].width + k];
					word.data[(y + j) * word.width * word.channels + (x + k) * word.channels + 2] = letters[i].data[j * letters[i].width + k];
					word.data[(y + j) * word.width * word.channels + (x + k) * word.channels + 3] = letters[i].data[j * letters[i].width + k];
				}
			}
			//Add space
			x += letters[i].width + spaceSizeBtLetters;
		}

		// Delete letters
		for (int i = 0; i < letters.size(); i++)
		{
			delete[] letters[i].data;
		}
	
		// Delete FreeType library
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
	else {
		//Create a blank image 1x1 pixel
		word.width = 1;
		word.height = 1;
		word.channels = 4;
		word.data = NULL;
		word.texture = -1;
		word.histogramR = NULL;
		word.histogramRSize = 0;
		word.histogramG = NULL;
		word.histogramGSize = 0;
		word.histogramB = NULL;
		word.histogramBSize = 0;
		word.modified = false;
		word.loaded = false;
	}
	
	return word;
}

void ImageWork::toolText(int _MouseImagePositionX, int _MouseImagePositionY) {
	//TODO
	this->textEnable = true;
	this->text = "Hello World!";
	this->textPosition.first = _MouseImagePositionX;
	this->textPosition.second = _MouseImagePositionY;
	int size = 500;
	std::string font = this->fontsPath + "/Arial.ttf";
	std::replace(font.begin(), font.end(), '\\', '/');
	ImageStr word = rasterText(this->text.c_str(), font.c_str(), size, size / 16, size / 4, RGB(0, 0, 0));
	this->addImage(word);
	this->combineLayers();
}