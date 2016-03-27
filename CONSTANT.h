#pragma once

#include <string>


void convert2WStr(wchar_t* dest, const char* src){
	size_t length;
	setlocale(LC_CTYPE, "jpn");
	mbstowcs_s(&length, dest, 256, src, 256);
}


void convert2Str(char* dest, const wchar_t* src){
	size_t length;
	setlocale(LC_CTYPE, "jpn");
	wcstombs_s(&length, dest, 256, src, 256);
}