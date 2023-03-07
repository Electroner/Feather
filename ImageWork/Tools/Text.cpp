#include "../ImageWork.h"

ImageStr rasterText(const char* text, const char* fontPath, int size, const RGB& color)
{
	int totalWidth = 0;
	int totalHeightUpper = 0;
	int totalHeightLower = 0;
	std::vector<ImageStr> letters;
	std::vector<int> bitmap_tops; //numbers of pixels from the baseline to the top of the glyph bitmap
	std::vector<int> bitmap_lefts;
	std::vector<int> advances;
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
		for (int i = 0; i < strlen(text); i++) {
			//Create the glyph of the letter and add to the vector with the bitmaps and the advances

			FT_Load_Char(face, text[i], FT_LOAD_RENDER);

			//Create the image of the letter
			ImageStr letter;
			letter.width = face->glyph->bitmap.width;
			letter.height = face->glyph->bitmap.rows;
			letter.channels = 1;
			letter.data = NULL;
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

			if (text[i] != ' ') {
				letter.data = new GLubyte[letter.width * letter.height * letter.channels];

				// Copy glyph bitmap to image
				for (int y = 0; y < letter.height; y++)
				{
					for (int x = 0; x < letter.width; x++)
					{
						letter.data[y * letter.width + x] = face->glyph->bitmap.buffer[y * letter.width + x];
					}
				}
			}

			// Add image to vector
			letters.push_back(letter);
			// Add width to total width
			totalWidth += (face->glyph->advance.x >> 6); //- face->glyph->bitmap_left;
			// Get the maximun over the baseline
			if (face->glyph->bitmap_top > totalHeightUpper)
			{
				totalHeightUpper = face->glyph->bitmap_top;
			}
			// Get the maximun under the baseline
			if ((static_cast<int>(face->glyph->bitmap.rows) - face->glyph->bitmap_top) >= totalHeightLower)
			{
				totalHeightLower = (static_cast<int>(face->glyph->bitmap.rows) - face->glyph->bitmap_top);
			}
			// Add the number of pixels from the baseline to the top of the glyph bitmap
			bitmap_tops.push_back(face->glyph->bitmap_top);
			// Add the number of pixels from the left border of the bitmap to the left border of the glyph
			bitmap_lefts.push_back(face->glyph->bitmap_left);
			// Add the advance of the letter
			advances.push_back((face->glyph->advance.x >> 6));
		}

		totalWidth += letters[letters.size() - 1].width - advances[advances.size() - 1] + bitmap_lefts[bitmap_lefts.size() - 1];

		//Create the image of the word
		word.width = totalWidth;
		word.height = totalHeightUpper + totalHeightLower;
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

		//Get the float value of the color
		float r = color.r / 255.0f;
		float g = color.g / 255.0f;
		float b = color.b / 255.0f;
		float a = color.delta / 255.0f;

		//Iterate through the letters
		//Copy the letter to the word following the advances and the bitmap_tops of the letters
		int x = 0;
		int y = 0;
		for (int i = 0; i < letters.size(); i++)
		{
			//Get the position of the letter
			y = totalHeightUpper - bitmap_tops[i];
			
			if (letters[i].data != NULL) {
				//Copy the letter to the word
				for (int j = 0; j < letters[i].height; j++)
				{
					for (int k = 0; k < letters[i].width; k++)
					{
						int index = (y + j) * word.width * word.channels + (x + k) * word.channels;
						GLubyte letter_color = letters[i].data[j * letters[i].width + k];

						//Copy the pixel
						word.data[index] = static_cast<unsigned char>(letter_color * r);
						word.data[index + 1] = static_cast<unsigned char>(letter_color * g);
						word.data[index + 2] = static_cast<unsigned char>(letter_color * b);
						word.data[index + 3] = static_cast<unsigned char>(letter_color * a);
					}
				}
			}
			x += advances[i]; //- bitmap_lefts[i];
			
			std::cout << "Letter: " << letters[i].name << std::endl;
			std::cout << "advace: " << advances[i] << std::endl;
			std::cout << "bitmap_top: " << bitmap_tops[i] << std::endl;
			std::cout << "bitmap_left: " << bitmap_lefts[i] << std::endl;
			std::cout << "width: " << letters[i].width << std::endl;
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
	else
	{
		//IF not correct
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
	this->text = "jkg lif";
	this->textPosition.first = _MouseImagePositionX;
	this->textPosition.second = _MouseImagePositionY;
	int size = 500;
	std::string font = this->fontsPath + "/Arial.ttf";
	std::replace(font.begin(), font.end(), '\\', '/');
	ImageStr word = rasterText(this->text.c_str(), font.c_str(), size, RGB(255, 255, 255, 255));
	this->addImage(word);
	this->combineLayers();
}