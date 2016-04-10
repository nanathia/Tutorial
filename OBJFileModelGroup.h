#pragma once
#include <string>

namespace obj{

	class OBJFileModelGroup
	{
		// グループ名
		std::string m_groupName = "";
		// 頂点バッファ開始地点
		int m_vertexStart;
		// 頂点バッファ使用サイズ
		int m_vertexSize;
		// インデクスバッファ開始地点
		int m_indexStart;
		// インデクスバッファ使用サイズ
		int m_indexSize;

	public:
		void setVertexStart(int index);
		void setVertexSize(int size);
		void setIndexStart(int index);
		void setIndexSize(int size);
		void setGroupName(const std::string& name);

		int getVertexStart() const;
		int getVertexSize() const;
		int getIndexStart() const;
		int getIndexSize() const;
		const std::string& getGroupName() const;

		OBJFileModelGroup();
		~OBJFileModelGroup();
	};

}