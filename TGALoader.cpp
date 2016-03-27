/*************************************************************************
　　TGALoader.cpp

　　TGA Texture 

　　Date : October 10, 2007
　　Version : 1.0
　　Author : Pocol
　　Memo :
*************************************************************************/

//
//　include
//
#include <fstream>
#include "TGALoader.h"

//////////////////////////////////////////////////////////////////////////
//　　TGAImage class
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------
//　　TGAImage
//　　Desc : コンストラクタ
//-----------------------------------------------------------------------------------------------------
TGAImage::TGAImage()
{
	imageSize = 0;
	imageData = NULL;
	format = GL_RGB;
	internalFormat = GL_RGB;
	width = 0;
	height = 0;
	bpp = 0;
	ID = 0;
}

//-----------------------------------------------------------------------------------------------------
//　　~TGAImage
//　　Desc : デストラクタ
//-----------------------------------------------------------------------------------------------------
TGAImage::~TGAImage()
{
	if ( imageData )
	{
		delete [] imageData;
		imageData = NULL;
	}
}

//-----------------------------------------------------------------------------------------------------
//　　ReadTGA
//　　Desc : TGAファイルの読み込み
//-----------------------------------------------------------------------------------------------------
bool TGAImage::ReadTGA(const char *filename)
{
	FILE *fp;
	GLubyte header[18]; 
	GLubyte bytePerPixel;
	GLuint temp;

	//　ファイルを開く
	if ( (fp = fopen(filename,"rb")) == NULL )
	{
		cout << "Error : 指定したファイルを開けませんでした\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	//　ヘッダー情報の読み込み
	fread(header, 1, sizeof(header), fp);
	
	//　幅と高さを決める
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	
	//　ビットの深さ
	bpp = header[16];

	//　24 bit
	if ( bpp == 24 )
	{
		format = GL_RGB;
		internalFormat = GL_RGB;
	}
	//　32 bit
	else if ( bpp = 32 )
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA;
	}

	//　1ピクセル当たりのバイト数を決定
	bytePerPixel = bpp/8;

	//　データサイズの決定
	imageSize = width * height * bytePerPixel;

	//　メモリを確保
	imageData = new GLubyte[imageSize];

	//　テクセルデータを一気に読み取り
	fread(imageData, 1, imageSize, fp);

	//　BGR(A)をRGB(A)にコンバート
	for ( int i=0; i<(int)imageSize; i+=bytePerPixel )
	{
		temp = imageData[i];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;
	}

	//　ファイルを閉じる
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------------
//　　Load
//　　Desc : TGAファイルを読み込み，テクスチャを生成
//-----------------------------------------------------------------------------------------------------
GLuint TGAImage::Load(const char *filename)
{
	//　ファイル読み込み
	if ( !ReadTGA(filename) )
		return false;

	//　テクスチャを生成
	glGenTextures(1, &ID);

	//　テクスチャをバインド
	glBindTexture(GL_TEXTURE_2D, ID);

	//　
	if ( bpp == 24 ) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	else if ( bpp == 32 ) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//　テクスチャの割り当て
	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData);

	//　テクスチャを拡大・縮小する方法の指定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//　テクスチャ環境
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//　メモリ解放
	if ( imageData )
	{
		delete [] imageData;
		imageData = NULL;
	}
	return ID;
}