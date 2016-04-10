#pragma once
#include "Ref.h"
#include "OBJModelFile.h"

class NomalModel :
	public Ref
{
	obj::OBJModelFile m_file;

public:
	NomalModel(const char* directoryName, const char* modelName);
	~NomalModel();

	void draw();
};

