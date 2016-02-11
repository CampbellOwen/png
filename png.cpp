#include "png.h"

PNG::PNG()
{
}

PNG::PNG(std::string filename)
{
	loadFile(filename);
}

PNG::~PNG()
{
	//delete[] img;
}

void PNG::loadFile(std::string filename)
{
	file = new std::ifstream(filename.c_str(), std::ios::in | std::ios::binary);

	size_t size = 0;
	file->seekg(0, std::ios::end);
	size = file->tellg();
	file->seekg(0, std::ios::beg);

	data = new unsigned char[size];
	file->read((char*)data, size);

	/*
	for (int i = 0; i < size; i++) {
		std::cout << std::hex;
		std::cout.width(2);
		std::cout.fill('0');
		std::cout << (int)data[i] << std::endl;
	}
	*/

	size_t index = 8; // ignore header
	chunk* chunks = new chunk[size];

	size_t chunkindex = 0;
	while (index < size) {
		for (int i = 0; i < 4; i++) {
			chunks[chunkindex].length[i] = data[index++];
		}
		std::cout << "Size of Chunk " << chunkindex+1 << ": " << char2int(chunks[chunkindex].length, 0) << std::endl;
		chunks[chunkindex].data = new unsigned char[char2int(chunks[chunkindex].length, 0)];

		for (int i = 0; i < 4; i++) {
			chunks[chunkindex].type[i] = data[index++];
		}

		if ( char2int(chunks[chunkindex].length, 0) != 0) {
			for (int i = 0; i < char2int(chunks[chunkindex].length, 0); i++) {
				chunks[chunkindex].data[i] = data[index++];
			}
		}

		for (int i = 0; i < 4; i++) {
			chunks[chunkindex].CRC[i] = data[index++];
		}

		chunkindex++;
	}

	for(int i = 0; i < chunkindex; i++) {
		char name[5];
		for (int j = 0; j < 4; j++) {
			name[j] = chunks[i].type[j];
		}
		name[4] = '\0';
		
		if (strcmp(name, "IHDR") == 0) {
			std::cout << "FOUND IHDR" << std::endl;
			width = char2int(chunks[i].data, 0);
			height = char2int(chunks[i].data, 4);
			bitdepth = chunks[i].data[8];
			colourtype = chunks[i].data[9];
			compressionmethod = chunks[i].data[10];
			filtermethod = chunks[i].data[11];
			interlacemethod = chunks[i].data[12];

		}
		else if (strcmp(name, "PLTE") == 0) {
			std::cout << "FOUND PLTE" << std::endl;
		}
		else if (strcmp(name, "IDAT") == 0) {
			std::cout << "FOUND IDAT" << std::endl;
		}
		else if (strcmp(name, "IEND") == 0) {
			std::cout << "FOUND IEND" << std::endl;
		}

	}
	

	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;

	delete[] chunks;
	
}

pixel* PNG::get()
{
	return img;
}

int PNG::char2int(unsigned char* arr, int start)
{
	int result = 0;
	for (int i = 0; i < 4; i++) {
		result += arr[start + i] << (24 - i * 8);
	}

	return result;
}
