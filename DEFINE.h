#pragma once

// �S�Ă̏ꍇ����ėp�I�ɌĂ΂ꂤ��֐��E�}�N���������ɏW�񂵂܂��B

#include <string>

namespace unknown{
	enum Exception{
		EXCEPTION_IGNORE,
		EXCEPTION_EXIT,
	};
	void halt(const char* filename, int line, const char* message);
}

// �f�o�O�ƃ����[�X���l�����Ȃ�ASSERT
#define STRONG_ASSERT( exp ) ( ( !! ( exp ) ) || ( unknown::halt( __FILE__, __LINE__, #exp ), 0 ) )
// �X�e�[�^�X�Ɋւ�炸�v���O�������~���܂��B
#define HALT( exp ) { unknown::halt( __FILE__, __LINE__, #exp ); }
// ���S�ȃf���[�g�ł��B
#define SAFE_DELETE( x ) { delete ( x ); ( x ) = 0; }
// ���S�Ȕz��f���[�g�ł��B
#define SAFE_DELETE_ARRAY( x ) { delete[] ( x ); ( x ) = 0; }
#define OPERATOR_DELETE( x ) { operator delete( x ); ( x ) = 0; }

//�f�o�O�ƃ����[�X�ŕ��򂷂����
#ifndef NDEBUG
#define NEW new( __FILE__, __LINE__ )
#define OPERATOR_NEW( x ) operator new( ( x ), __FILE__, __LINE__ ) 
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( unknown::halt( __FILE__, __LINE__, #exp ), 0 ) )
#else //NDEBUG
#define NEW new
#define OPERATOR_NEW( x ) operator new( x ) 
#define ASSERT( exp ) 
#endif //NDEBUG

// ���C�h�����A�}���`�o�C�g�����̕ϊ�
void convert2WStr(wchar_t* dest, const char* src);
void convert2Str(char* dest, const wchar_t* src);

// �e�X�g�t���O
#define TEST_SIZURU

#define M_PI 3.14159265359