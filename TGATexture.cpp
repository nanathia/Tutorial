#include "TGATexture.h"
#include <fstream>
#include "INCLUDES.h"
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
			// 色深度32ビット（透明度付き）
			for (int y = 0; y < m_height; y++){
				int y_reverse = m_height - y - 1;
				for (int x = 0; x < m_width; x++){
					// GL仕様で上下が逆転しているため、調整しています。
					int index = y_reverse * m_width + x;
					unsigned char* temp = reinterpret_cast<unsigned char*>(&m_data[index]);
					temp[2] = binary[index++];
					temp[1] = binary[index++];
					temp[0] = binary[index++];
					temp[3] = binary[index++];
				}
			}
		}
		else if (colorDepth == 24){
			// 色深度24ビット（透明度なし）
			for (int y = 0; y < m_height; y++){
				int y_reverse = m_height - y - 1;
				for (int x = 0; x < m_width; x++){
					// GL仕様で上下が逆転しているため、調整しています。
					int index = y_reverse * m_width + x;
					unsigned char* temp = reinterpret_cast<unsigned char*>(&m_data[index]);
					temp[2] = binary[index++];
					temp[1] = binary[index++];
					temp[0] = binary[index++];
					temp[3] = 0xff;
				}
			}
		}
		delete[] binary;
	}
	else{
		HALT(TGAファイルを読めませんでした。);
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