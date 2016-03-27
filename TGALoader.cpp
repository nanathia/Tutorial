/*************************************************************************
�@�@TGALoader.cpp

�@�@TGA Texture 

�@�@Date : October 10, 2007
�@�@Version : 1.0
�@�@Author : Pocol
�@�@Memo :
*************************************************************************/

//
//�@include
//
#include <fstream>
#include "TGALoader.h"

//////////////////////////////////////////////////////////////////////////
//�@�@TGAImage class
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------
//�@�@TGAImage
//�@�@Desc : �R���X�g���N�^
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
//�@�@~TGAImage
//�@�@Desc : �f�X�g���N�^
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
//�@�@ReadTGA
//�@�@Desc : TGA�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------------------------------------
bool TGAImage::ReadTGA(const char *filename)
{
	FILE *fp;
	GLubyte header[18]; 
	GLubyte bytePerPixel;
	GLuint temp;

	//�@�t�@�C�����J��
	if ( (fp = fopen(filename,"rb")) == NULL )
	{
		cout << "Error : �w�肵���t�@�C�����J���܂���ł���\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	//�@�w�b�_�[���̓ǂݍ���
	fread(header, 1, sizeof(header), fp);
	
	//�@���ƍ��������߂�
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	
	//�@�r�b�g�̐[��
	bpp = header[16];

	//�@24 bit
	if ( bpp == 24 )
	{
		format = GL_RGB;
		internalFormat = GL_RGB;
	}
	//�@32 bit
	else if ( bpp = 32 )
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA;
	}

	//�@1�s�N�Z��������̃o�C�g��������
	bytePerPixel = bpp/8;

	//�@�f�[�^�T�C�Y�̌���
	imageSize = width * height * bytePerPixel;

	//�@���������m��
	imageData = new GLubyte[imageSize];

	//�@�e�N�Z���f�[�^����C�ɓǂݎ��
	fread(imageData, 1, imageSize, fp);

	//�@BGR(A)��RGB(A)�ɃR���o�[�g
	for ( int i=0; i<(int)imageSize; i+=bytePerPixel )
	{
		temp = imageData[i];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;
	}

	//�@�t�@�C�������
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------------
//�@�@Load
//�@�@Desc : TGA�t�@�C����ǂݍ��݁C�e�N�X�`���𐶐�
//-----------------------------------------------------------------------------------------------------
GLuint TGAImage::Load(const char *filename)
{
	//�@�t�@�C���ǂݍ���
	if ( !ReadTGA(filename) )
		return false;

	//�@�e�N�X�`���𐶐�
	glGenTextures(1, &ID);

	//�@�e�N�X�`�����o�C���h
	glBindTexture(GL_TEXTURE_2D, ID);

	//�@
	if ( bpp == 24 ) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	else if ( bpp == 32 ) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//�@�e�N�X�`���̊��蓖��
	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData);

	//�@�e�N�X�`�����g��E�k��������@�̎w��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//�@�e�N�X�`����
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//�@���������
	if ( imageData )
	{
		delete [] imageData;
		imageData = NULL;
	}
	return ID;
}