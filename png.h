#include <string>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <string.h>

typedef struct {
	char r;
	char g;
	char b;
	char a;
} pixel;

class chunk {
	public:
		unsigned char length[4];
		unsigned char type[4];
		unsigned char* data;
		unsigned char CRC[4];

		~chunk()
		{
			delete[] data;
		}
};

class PNG 
{
	public:
		PNG();
		PNG(std::string filename);
		~PNG();
		void loadFile(std::string filename);
		pixel* get();
	private:
		pixel* img;
		int width;
		int height;
		int bitdepth;
		int colourtype;
		int compressionmethod;
		int filtermethod;
		int interlacemethod;

		unsigned char* data;
		std::ifstream* file;

		int char2int(unsigned char* arr, int start);
};
