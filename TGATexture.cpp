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

		// �摜�T�C�Y
		m_width = *reinterpret_cast<unsigned short*>(&binary[index]);
		index += 2;
		m_height = *reinterpret_cast<unsigned short*>(&binary[index]);
		index += 2;

		// �F�[�x
		unsigned char colorDepth = *reinterpret_cast<unsigned char*>(&binary[index]);
		index += 2;
		m_data = new unsigned[m_width*m_height];
		int rect = m_width * m_height;

		if (colorDepth == 32){
			// �F�[�x32�r�b�g�i�����x�t���j
			for (int y = 0; y < m_height; y++){
				int y_reverse = m_height - y - 1;
				for (int x = 0; x < m_width; x++){
					// GL�d�l�ŏ㉺���t�]���Ă��邽�߁A�������Ă��܂��B
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
			// �F�[�x24�r�b�g�i�����x�Ȃ��j
			for (int y = 0; y < m_height; y++){
				int y_reverse = m_height - y - 1;
				for (int x = 0; x < m_width; x++){
					// GL�d�l�ŏ㉺���t�]���Ă��邽�߁A�������Ă��܂��B
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
		HALT(TGA�t�@�C����ǂ߂܂���ł����B);
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