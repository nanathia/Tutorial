#pragma once

// このプロジェクトのどこからでもアクセス可能な場所です。

class GyuDon;

class Director
{
	static Director* m_instance;

	Director();
	~Director();

public:

	static void create();
	static void destroy();
	static Director* instance();


// ここから実体です。
private:
	GyuDon* m_framework = 0;

public:
	GyuDon* framework();
};