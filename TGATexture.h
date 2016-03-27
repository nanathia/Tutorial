#pragma once


class TGA{
	unsigned* m_data;
	int m_width;
	int m_height;
public:
	TGA(const char* file);
	~TGA();
	unsigned* GetImage();
	int GetWidth();
	int GetHeight();
};