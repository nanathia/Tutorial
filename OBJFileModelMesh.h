#pragma once
#include "DEFINE.h"
#include "MTLFileModelMaterial.h"

namespace obj{

	// .objファイルの描画単位です。

	class OBJFileModelMesh
	{
		// マテリアル名
		std::string m_materialName = "";
		// 頂点バッファ開始地点
		int m_vertexStart;
		// 頂点バッファ使用サイズ
		int m_vertexSize;
		// インデクスバッファ開始地点
		int m_indexStart;
		// インデクスバッファ使用サイズ
		int m_indexSize;
		// マテリアルのポインタ
		MTLFileModelMaterial* m_material = 0;

	public:

		void setVertexStart(int index);
		void setVertexSize(int size);
		void setIndexStart(int index);
		void setIndexSize(int size);
		void setMaterialName(const std::string& name);

		int getVertexStart() const;
		int getVertexSize() const;
		int getIndexStart() const;
		int getIndexSize() const;
		const std::string& getMaterialName() const;

		void setMaterial(MTLFileModelMaterial* material);
		MTLFileModelMaterial* getMaterial();

		OBJFileModelMesh();
		~OBJFileModelMesh();

	};

}