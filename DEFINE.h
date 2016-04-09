#pragma once

// 全ての場合から汎用的に呼ばれうる関数・マクロをここに集約します。

#include <string>

namespace unknown{
	enum Exception{
		EXCEPTION_IGNORE,
		EXCEPTION_EXIT,
	};
	void halt(const char* filename, int line, const char* message);
}

// デバグとリリースを考慮しないASSERT
#define STRONG_ASSERT( exp ) ( ( !! ( exp ) ) || ( unknown::halt( __FILE__, __LINE__, #exp ), 0 ) )
// ステータスに関わらずプログラムを停止します。
#define HALT( exp ) { unknown::halt( __FILE__, __LINE__, #exp ); }
// 安全なデリートです。
#define SAFE_DELETE( x ) { delete ( x ); ( x ) = 0; }
// 安全な配列デリートです。
#define SAFE_DELETE_ARRAY( x ) { delete[] ( x ); ( x ) = 0; }
#define OPERATOR_DELETE( x ) { operator delete( x ); ( x ) = 0; }

//デバグとリリースで分岐するもの
#ifndef NDEBUG
#define NEW new( __FILE__, __LINE__ )
#define OPERATOR_NEW( x ) operator new( ( x ), __FILE__, __LINE__ ) 
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( unknown::halt( __FILE__, __LINE__, #exp ), 0 ) )
#else //NDEBUG
#define NEW new
#define OPERATOR_NEW( x ) operator new( x ) 
#define ASSERT( exp ) 
#endif //NDEBUG

// ワイド文字、マルチバイト文字の変換
void convert2WStr(wchar_t* dest, const char* src);
void convert2Str(char* dest, const wchar_t* src);

// テストフラグ
#define TEST_SIZURU

#define M_PI 3.14159265359