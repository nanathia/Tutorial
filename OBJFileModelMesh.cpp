#include "OBJFileModelMesh.h"


namespace obj{

	OBJFileModelMesh::OBJFileModelMesh()
	{
	}


	OBJFileModelMesh::~OBJFileModelMesh()
	{
	}

	void OBJFileModelMesh::setVertexStart(int index){
		m_vertexStart = index;
	}
	void OBJFileModelMesh::setVertexSize(int size){
		m_vertexSize = size;
	}
	void OBJFileModelMesh::setIndexStart(int index){
		m_indexStart = index;
	}
	void OBJFileModelMesh::setIndexSize(int size){
		m_indexSize = size;
	}
	void OBJFileModelMesh::setMaterialName(const std::string& name){
		m_materialName = name;
	}

	int OBJFileModelMesh::getVertexStart() const{
		return m_vertexStart;
	}
	int OBJFileModelMesh::getVertexSize() const{
		return m_vertexSize;
	}
	int OBJFileModelMesh::getIndexStart() const{
		return m_indexStart;
	}
	int OBJFileModelMesh::getIndexSize() const{
		return m_indexSize;
	}
	const std::string& OBJFileModelMesh::getMaterialName() const{
		return m_materialName;
	}

	void OBJFileModelMesh::setMaterial(MTLFileModelMaterial* material){
		m_material = material;
	}
	MTLFileModelMaterial* OBJFileModelMesh::getMaterial(){
		return m_material;
	}

}