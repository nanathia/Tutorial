#include "OBJFileModelGroup.h"
#include "DEFINE.h"

namespace obj{

	OBJFileModelGroup::OBJFileModelGroup()
	{
	}


	OBJFileModelGroup::~OBJFileModelGroup()
	{
	}


	void OBJFileModelGroup::setVertexStart(int index){
		m_vertexStart = index;
	}
	void OBJFileModelGroup::setVertexSize(int size){
		m_vertexSize = size;
	}
	void OBJFileModelGroup::setIndexStart(int index){
		m_indexStart = index;
	}
	void OBJFileModelGroup::setIndexSize(int size){
		m_indexSize = size;
	}
	void OBJFileModelGroup::setGroupName(const std::string& name){
		ASSERT(m_groupName == "" && "同じグループに、二つ目のマテリアルが適用されようとしています。");
		m_groupName = name;
	}

	int OBJFileModelGroup::getVertexStart() const{
		return m_vertexStart;
	}
	int OBJFileModelGroup::getVertexSize() const{
		return m_vertexSize;
	}
	int OBJFileModelGroup::getIndexStart() const{
		return m_indexStart;
	}
	int OBJFileModelGroup::getIndexSize() const{
		return m_indexSize;
	}
	const std::string& OBJFileModelGroup::getGroupName() const{
		return m_groupName;
	}

}