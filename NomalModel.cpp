#include "NomalModel.h"


NomalModel::NomalModel(const char* directoryName, const char* modelName)
{
	m_file.Load(directoryName, modelName);
	int i = 0;
}


NomalModel::~NomalModel()
{
}