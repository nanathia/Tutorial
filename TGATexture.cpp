#include "TGATexture.h"
#include <fstream>
using namespace std;

TGA::TGA(const char* file){
	ifstream ifs(file, ifstream::binary);
	if (ifs){
		ifs.seekg(0, ifstream::end);
		int size = ifs.tellg();
		char* binary = new char[size];
		ifs.seekg(0, ifstream::beg);
		ifs.read(binary, size);
		int index = 0;
		index += 12;

		// 画像サイズ
		m_width = *reinterpret_cast<unsigned short*>(&binary[index]);
		index += 2;
		m_height = *reinterpret_cast<unsigned short*>(&binary[index]);
		index += 2;

		// 色深度
		unsigned char colorDepth = *reinterpret_cast<unsigned char*>(&binary[index]);
		index += 2;
		m_data = new unsigned[m_width*m_height];
		int rect = m_width * m_height;

		if (colorDepth == 32){
			for (int i = 0; i < rect; i++){
				unsigned char* temp = reinterpret_cast<unsigned char*>(&m_data[i]);
				temp[2] = binary[index++];
				temp[1] = binary[index++];
				temp[0] = binary[index++];
				temp[3] = binary[index++];
			}
		}
		else if (colorDepth == 24){
			for (int i = 0; i < rect; i++){
				unsigned char* temp = reinterpret_cast<unsigned char*>(&m_data[i]);
				temp[2] = binary[index++];
				temp[1] = binary[index++];
				temp[0] = binary[index++];
				temp[3] = 0x00;
			}
		}
		delete[] binary;
	}
}

TGA::~TGA(){
	delete[] m_data;
}

unsigned* TGA::GetImage(){
	return m_data;
}

int TGA::GetWidth(){
	return m_width;
}

int TGA::GetHeight(){
	return m_height;
}