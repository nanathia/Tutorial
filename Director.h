#pragma once

// ���̃v���W�F�N�g�̂ǂ�����ł��A�N�Z�X�\�ȏꏊ�ł��B

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


// ����������̂ł��B
private:
	GyuDon* m_framework = 0;

public:
	GyuDon* framework();
};