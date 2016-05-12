#pragma once

#include <string>
#include "OBJFileModelGroup.h"
#include "OBJFileModelMesh.h"
#include "MTLFileModelMaterial.h"
#include "MyVector.h"
#include <vector>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

namespace obj{

	class OBJModelFile
	{
		// 頂点データ
		std::vector<Vec3> m_destxyz;
		std::vector<Vec3> m_destnomal;
		std::vector<Vec2> m_destuv;
		std::vector<unsigned> m_destIndexBuf;

	private:
		std::string m_modelName;
		std::string m_directoryName;
		// 頂点を一つ読む
		void vertexRead(char*& data);
		// 次の行まで読み飛ばす
		void goNextLine(char*& data);
		// 次のスペースまで読み飛ばす。スペースが無かった場合はfalseを返す。
		bool goNextSpace(char*& data);
		// 次のスペースまで読みとばしつつもそれまでの文字列を取得 スペースが無かった場合はfalseを返す。
		bool goNextSpaceAndGetStr(char*& data, char*& str);
		// 次のスラッシュ'/'まで読み飛ばす。スペースまでにスラッシュが無ければfalseを返す。
		bool goNextSlash(char*& data);
		// 次のスラッシュまで読み飛ばしつつそれまでの文字列を取得　スペースまでにスラッシュがなかった場合そこで終了しfalseを返す
		bool goNextSlashAndGetStr(char*& data, char*& str);
		// その文字が、符号なし数字であるかどうか。
		bool isCharcterUnsignedNumber(char c) const;
		// 符号なし整数文字列を取得しつつ読み飛ばす。符号なし整数以外の文字が来るまでの長さの文字列とする。最後にたどり着いた文字を返す。
		char getUnsignedNumberStr(char*& binary, char*& str);
		// faceデータ専用で、何角形か調べつつデータを読み進める。改行後まで進むことは無いことが保証される。
		int getCountFaceVertexNumByDataLine(char*& binary);
		// 名前などに使える文字以外がくるまで読み飛ばしつつ取得する。最後に来た文字を返す。
		char goNextBorderAndGetNameStr(char*& data, char*& str);
		// 名前などにそのまま使用できる文字かどうかを判定する
		bool isCanUseNamingCharacter(char c);
		// 符号なし浮動小数点に使える文字以外がくるまで読み飛ばしつつ取得する
		char goNextBorderAndGetUnsignedFloatStr(char*& binary, char*& str);

		// .obj
		// データの中のそれぞれの要素の数を数える。
		void countDataXYZ_UV_Nomal_Face(char* data, int& xyz, int& uv, int& nomal, int& faceNum, int& vertBuf, int& indexBuf, int& groupNum, int& meshNum);
		// 面毎の頂点数を記録する。それから、必要なデータインデクスの、書き込む領域も確保する。
		void countVertexForFace(char* binary, std::vector<std::vector<Vec3I>>& dataIndexList, std::vector<OBJFileModelGroup>& groupList, std::vector<OBJFileModelMesh>& meshList);
		// 頂点バッファとインデクスバッファを取得する。
		void getVertexAndIndexBuffer(std::vector<Vec3>& srcXYZ, std::vector<Vec2>& srcUV, std::vector<Vec3>& srcNomal, std::vector<Vec3>& destXYZ, std::vector<Vec2>& destUV, std::vector<Vec3>& destNomal, std::vector<std::vector<Vec3I>>& dataIndexData, std::vector<unsigned>& destIndexBuf);
		// .objデータをロードする。
		void loadObjData(char* binary, std::vector<Vec3>& xyz, std::vector<Vec3>& nomal, std::vector<Vec2>& uv, std::vector<std::vector<Vec3I>>& dataIndexList);

		// .mtl
		// マテリアルの数を数える。
		void countMaterialByBinaryData(char* binary, int& materialNum);
		// マテリアルをセットする。
		void setMaterialByBinaryData(char* binary, std::vector<MTLFileModelMaterial>& materialList);

		// .objファイルに関する処理
		void OBJDataLoadAndSetting();
		// .mtlファイルに関する処理
		void MTLDataLoadAndSetting();

		// 最終処理。
		void setMeshByMaterialData();

		// このモデルの、三角形の数
		int m_primitiveCount = -1;
		// このモデルの、頂点の数。
		int m_vertexCount = -1;

		// このモデル内のグループのリスト
		std::vector<OBJFileModelGroup> m_groupList;
		// このモデル内のメッシュのリスト
		std::vector<OBJFileModelMesh> m_meshList;
		// このモデル内のマテリアルのリスト
		std::vector<MTLFileModelMaterial> m_materialList;
		// このモデル内のテクスチャのリスト
		std::vector<std::string> m_textureNameList;

	public:
		OBJModelFile();
		virtual ~OBJModelFile();

		void Load(const std::string& directory, const std::string& modelName);
		
		const std::vector<Vec3>& getPosition();
		const std::vector<unsigned>& getVertexIndex();
		const std::vector<Vec3>& getNormal();
		const std::vector<Vec2>& getUV();
		const std::vector<std::string> getTextureName();

		int vertexSize();
		int vertexIndexSize();
		int getTextureCount();

	};

}