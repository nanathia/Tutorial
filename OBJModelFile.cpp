#include "OBJModelFile.h"
#include <fstream>
#include <sstream>
#include "DEFINE.h"
#include <cstdlib>

namespace obj{

	OBJModelFile::OBJModelFile()
	{
	}


	OBJModelFile::~OBJModelFile()
	{
	}

	void OBJModelFile::Load(const std::string& directory, const std::string& modelName){
		m_modelName = modelName;
		m_directoryName = directory;
		// データ読み込み
		OBJDataLoadAndSetting();
		int i = 0;
	}

	// 頂点を一つ読む
	void OBJModelFile::vertexRead(char*& data){
	}

	// 次の行まで読み飛ばす
	void OBJModelFile::goNextLine(char*& data){
		while (true){
			if (*data++ == 0x0a){
				return;
			}
		}
	}

	// .mtlファイルに関する処理
	void OBJModelFile::MTLDataLoadAndSetting(){
		// .mtlファイルの読み込みです
		std::string name = m_directoryName + "/" + m_modelName + ".mtl";
		std::ifstream ifs(name.c_str(), std::fstream::binary);
		ASSERT(ifs && ".obj file not found");
		ifs.seekg(0, std::ifstream::end);
		int binarySize = (int)ifs.tellg();
		char* binary = new char[binarySize + 2];
		ifs.seekg(0, std::ifstream::beg);
		ifs.read(binary, binarySize);
		ifs.close();
		{
			// ファイル終端を付ける。ファイル終端に来た時に、解析ループを止めなければメモリリークするため。
			binary[binarySize] = 0x0a;
			binary[binarySize + 1] = '\0';
		}

		// まず、マテリアルの数を数える。
		int materialNum;
		countMaterialByBinaryData(binary, materialNum);

		// メッシュデータの配列を確保する。
		m_materialList.reserve(materialNum);

		// マテリアルをセットする
		setMaterialByBinaryData(binary, m_materialList);

		// ファイルデータは必要なくなったので解放。
		SAFE_DELETE_ARRAY(binary);
	}

	// .objファイルに関する処理
	void OBJModelFile::OBJDataLoadAndSetting(){
		{
			// .objファイルの読み込みです。
			std::string name = m_directoryName + "/" + m_modelName + ".obj";
			std::ifstream ifs(name.c_str(), std::fstream::binary);
			ASSERT(ifs && ".obj file not found");
			ifs.seekg(0, std::ifstream::end);
			int binarySize = (int)ifs.tellg();
			char* binary = new char[binarySize + 2];
			ifs.seekg(0, std::ifstream::beg);
			ifs.read(binary, binarySize);
			ifs.close();
			{
				// ファイル終端を付ける。ファイル終端に来た時に、解析ループを止めなければメモリリークするため。
				binary[binarySize] = 0x0a;
				binary[binarySize + 1] = '\0';
			}

			// 必要な要素の数を数えて保存しておく。
			int xyzNum, nomalNum, uvNum, faceNum, vertBufNum, IndexBufNum, groupNum, meshNum;
			countDataXYZ_UV_Nomal_Face(binary, xyzNum, uvNum, nomalNum, faceNum, vertBufNum, IndexBufNum, groupNum, meshNum);
			// 描画の時の為に、三角形の数を計上。インデクスバッファの数/3。
			m_primitiveCount = IndexBufNum / 3;
			// ３で割り切れないとおかしい。
			ASSERT(!(IndexBufNum % 3));
			// 同じく、頂点の数を保存する
			m_vertexCount = vertBufNum;
			// グループリストの配列を確保する。
			m_groupList.reserve(groupNum);
			// メッシュリストの配列を確保する。
			m_meshList.reserve(meshNum);

			// 面毎の頂点数を記録する。
			std::vector<std::vector<Vec3I>> dataIndexList;
			dataIndexList.reserve(faceNum);
			countVertexForFace(binary, dataIndexList, m_groupList, m_meshList);

			// データから、あらゆるデータを抜き出すためのバッファを用意する。
			std::vector<Vec3> xyz;
			xyz.reserve(xyzNum);
			std::vector<Vec3> normal;
			normal.reserve(nomalNum);
			std::vector<Vec2> uv;
			uv.reserve(uvNum);

			// データから、色々とを抜き出す。
			loadObjData(binary, xyz, normal, uv, dataIndexList);

			// ここで、元のデータは不要となるので解放。
			SAFE_DELETE_ARRAY(binary);

			// 頂点バッファ・インデクスバッファの作成
			m_destxyz.clear();
			m_destnomal.clear();
			m_destuv.clear();
			m_destIndexBuf.clear();
			m_destxyz.reserve(vertBufNum);
			m_destnomal.reserve(vertBufNum);
			m_destuv.reserve(vertBufNum);
			m_destIndexBuf.reserve(IndexBufNum);

			// 頂点バッファ・インデクスバッファをテンポラリバッファに読み込む。
			getVertexAndIndexBuffer(xyz, uv, normal, m_destxyz, m_destuv, m_destnomal, dataIndexList, m_destIndexBuf);

			// MTLを読み込み冪乗によるUV誤差を修正する。
			MTLDataLoadAndSetting();
			// 最終処理。
			setMeshByMaterialData();
			//if (m_meshList.size()){
			//	int size = m_meshList.size();
			//	for (int i = 0; i < size; i++){
			//		OBJFileModelMesh& mesh = m_meshList[i];
			//		MTLFileModelMaterial* material = mesh.getMaterial();
			//		int end = mesh.getVertexStart() + mesh.getVertexSize();
			//		Texture* tex = mesh.getMaterial()->getTexture();
			//		float uofs = tex->getWidth() / tex->getDefaultWidth();
			//		float vofs = tex->getHeight() / tex->getDefaultHeight();
			//		for (int s = mesh.getVertexStart(); s < end; ++s){
			//			destuv[s].x *= uofs;
			//			destuv[s].y *= vofs;
			//		}
			//	}
			//}
		}
	}

	// 最終処理。
	void OBJModelFile::setMeshByMaterialData(){
		if (m_meshList.size()){
			auto it = m_meshList.begin();
			while (it != m_meshList.end()){
				OBJFileModelMesh& mesh = *it;
				auto it2 = m_materialList.begin();
				while (it2 != m_materialList.end()){
					if (mesh.getMaterialName() == it2->getName()){
						mesh.setMaterial(&*it2);
					}
					it2++;
				}
				ASSERT(mesh.getMaterial());
				it++;
			}
			// テクスチャ切り替えの、出来るだけ少ないように並べ替える
			int size = m_meshList.size();
			for (int i = 0; i < size; i++){
				const std::string& tex = m_meshList[i].getMaterial()->getTextureName();
				for (int j = i + 1; j < size; j++){
					if (m_meshList[j].getMaterial()->getTextureName() == tex){
						std::swap(m_meshList[i + 1], m_meshList[j]);
						++i;
					}
				}
			}
		}

		std::string pre = "empty";
		for (int i = 0; i < m_meshList.size(); i++){
			if (m_meshList[i].getMaterial()->getTextureName() != pre && m_meshList[i].getMaterial()->getTextureName() != ""){
				pre = m_meshList[i].getMaterial()->getTextureName();
				m_textureNameList.push_back(pre);
			}
		}
	}

	// 頂点バッファとインデクスバッファを取得する。
	void OBJModelFile::getVertexAndIndexBuffer(
		std::vector<Vec3>& srcXYZ, std::vector<Vec2>& srcUV, std::vector<Vec3>& srcNomal,
		std::vector<Vec3>& destXYZ, std::vector<Vec2>& destUV, std::vector<Vec3>& destNomal,
		std::vector<std::vector<Vec3I>>& dataIndexData,
		std::vector<unsigned>& destIndexBuf)
	{
		// 作成した頂点の数を面毎に加算してゆく。
		int vertexCountAll = 0;
		// 面の数だけループを回す。
		for (auto srcIndex : dataIndexData){
			int vertexSize = srcIndex.size();
			// 法線を計算するかどうかのフラグ
			bool isNeedCreateNomal = false;
			// まずは、頂点バッファを作成する。頂点バッファの作成に関しては、面の頂点数は関与しない。
			for (int i = 0; i < vertexSize; i++){
				// .objデータからそのまま抜き出したデータのインデクス。座標、uv、法線の順。
				Vec3I& i2 = srcIndex[i];
				// 座標は必ず存在する。
				destXYZ.push_back(srcXYZ[i2.x]);
				// uvは存在しない可能性がある。
				if (i2.y == -1){
					// uvは存在しません。
					destUV.push_back(0.f);
				}
				else{
					// uvが存在します。
					destUV.push_back(srcUV[i2.y]);
				}
				// 法線も存在しない可能性がある。
				if (i2.z == -1){
					// 法線が存在しない。（あとでポリゴンの向きから計算する）
					isNeedCreateNomal = true;
				}
				else{
					// 法線が存在する。
					ASSERT(!isNeedCreateNomal);
					destNomal.push_back(srcNomal[i2.z]);
				}
			}
			// 頂点バッファ作成完了。
			// 法線計算の必要がある場合計算。
			if (isNeedCreateNomal){
				// ポリゴンを取得
				Vec3* poligon = &destXYZ[destXYZ.size() - vertexSize];
				// 計算に使用する２ベクトルを計算。三角形だろうと４角形だろうと変わらない。
				Vec3 v1 = poligon[1] - poligon[0];
				Vec3 v2 = poligon[2] - poligon[0];
				// 外積を取得。
				Vec3 cross;
				D3DXVec3Cross(cross.getD3DVector(), v1.getD3DVector_c(), v2.getD3DVector_c());
				// 正規化。
				D3DXVec3Normalize(cross.getD3DVector(), cross.getD3DVector_c());
				// 法線として適用。
				for (int i = 0; i < vertexSize; i++){
					destNomal.push_back(cross);
				}
			}
			// インデクスバッファの適用。
			if (vertexSize == 3){
				// ３面なら３インデクス。
				destIndexBuf.push_back(vertexCountAll);
				destIndexBuf.push_back(vertexCountAll + 1);
				destIndexBuf.push_back(vertexCountAll + 2);
				vertexCountAll += 3;
			}
			else if (vertexSize == 4){
				// 4面なら６インデクス
				destIndexBuf.push_back(vertexCountAll);
				destIndexBuf.push_back(vertexCountAll + 1);
				destIndexBuf.push_back(vertexCountAll + 2);
				destIndexBuf.push_back(vertexCountAll + 0);
				destIndexBuf.push_back(vertexCountAll + 2);
				destIndexBuf.push_back(vertexCountAll + 3);
				vertexCountAll += 4;
			}
			else{
				HALT(未対応の頂点数);
			}
		}
	}

	// 面毎の頂点数を記録する。それと、グループ毎の情報も取得する。
	void OBJModelFile::countVertexForFace(char* binary, std::vector<std::vector<Vec3I>>& dataIndexList, std::vector<OBJFileModelGroup>& group, std::vector<OBJFileModelMesh>& mesh){
		char str[512];
		char* p = str;
		// 現在までにプログラムが通った頂点の数の合計
		int runedVertexNum = 0;
		// 現在にプログラムが通ったインデクスバッファの数の合計。
		int runedIndexNum = 0;
		while (true){
			// 行の先頭文字によってループ内容を変更する。

			if (binary[0] == '\0'){
				if (group.size()){
					// グループが存在する。
					// 最後のグループを閉じる。
					group.back().setVertexSize(runedVertexNum - group.back().getVertexStart());
					group.back().setIndexSize(runedIndexNum - group.back().getIndexStart());
				}
				// メッシュもあれば閉じる。
				if (mesh.size()){
					mesh.back().setVertexSize(runedVertexNum - mesh.back().getVertexStart());
					mesh.back().setIndexSize(runedIndexNum - mesh.back().getIndexStart());
				}
				return;
			}

			// 先頭文字を取得
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);
			if (!strcmp("f", p)){
				// 面情報。面の頂点数を記録する。
				// 何角系か調べる。
				int count = getCountFaceVertexNumByDataLine(binary);
				if (count == 3){
					// 三角形です。
					dataIndexList.push_back(std::vector<Vec3I>());
					dataIndexList.back().resize(3);
					// プログラムが通った頂点、インデクスの合計を加算
					runedVertexNum += 3;
					runedIndexNum += 3;
				}
				else if (count == 4){
					// 四角形です。
					dataIndexList.push_back(std::vector<Vec3I>());
					dataIndexList.back().resize(4);
					// プログラムが通った頂点、インデクスの合計を加算
					runedVertexNum += 4;
					runedIndexNum += 6;
				}
				else{
					HALT(未対応の形です);
				}
			}
			else if (!strcmp(p, "g")){
				// グループを記録。

				// このグループのインスタンスを、グループリストの末尾に生成する。
				group.push_back(OBJFileModelGroup());
				// グループとしてまとめた合計の、次からが頂点のこのグループの開始地点となる。
				group.back().setVertexStart(runedVertexNum);
				// 上記と同様の事をインデクス側にも適用。
				group.back().setIndexStart(runedIndexNum);

				if (group.size() >= 2){
					// 前のグループが存在する。
					OBJFileModelGroup& prev = group[group.size() - 2];
					// 前のグループがあった場合、
					// 現在の通った頂点数引くそのグループの宣言があった場所の頂点数が、
					// そのグループの頂点サイズとなる。
					prev.setVertexSize(runedVertexNum - prev.getVertexStart());
					prev.setIndexSize(runedIndexNum - prev.getIndexStart());
				}

				// グループ名を読み飛ばしつつ取得。
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// 次の行の先頭まで来てしまったので、更に改行する必要が無くなった。
					isGoNextLine = false;
				}
				// グループのインスタンスに名前を設定
				group.back().setGroupName(p);

			}
			else if (!strcmp(p, "usemtl")){
				// メッシュの取得。

				// メッシュリストの末尾にインスタンスを生成
				mesh.push_back(OBJFileModelMesh());

				// グループリストと同じように適用する。※このブロックはグループの場合と全く同じ処理
				mesh.back().setVertexStart(runedVertexNum);
				mesh.back().setIndexStart(runedIndexNum);
				if (mesh.size() >= 2){
					OBJFileModelMesh& prev = mesh[mesh.size() - 2];
					prev.setVertexSize(runedVertexNum - prev.getVertexStart());
					prev.setIndexSize(runedIndexNum - prev.getIndexStart());
				}

				// マテリアル名を、読み飛ばしつつ取得する
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// 次の行の先頭まで来てしまったので、最後に改行する必要がなくなった
					isGoNextLine = false;
				}
				// メッシュリストの一番末尾のメッシュに、マテリアル名を適用。
				mesh.back().setMaterialName(p);
			}

			// 改行
			if (isGoNextLine){
				goNextLine(binary);
			}
		}
	}

	// マテリアルの数を数える。
	void OBJModelFile::countMaterialByBinaryData(char* data, int& materialNum){
		char str[256];
		char* p = str;
		char error[128];
		char* e = error;
		materialNum = 0;

		while (true){

			if (*data == '\0'){
				// データの終端まで来たので終了。
				return;
			}

			// 先頭文字列を取得する。
			bool isGoNextLine = goNextSpaceAndGetStr(data, p);

			if (!strcmp(p, "newmtl")){
				// 新規マテリアル名の宣言行です。
				++materialNum;
			}

			if (isGoNextLine){
				goNextLine(data);
			}
		}
	}

	// マテリアルをセットする。
	void OBJModelFile::setMaterialByBinaryData(char* binary, std::vector<MTLFileModelMaterial>& material){
		char str[256];
		char* p = str;
		char error[128];
		char* e = error;

		while (true){

			if (*binary == '\0'){
				// データの終端まで来たので終了。
				return;
			}

			// 先頭文字列を取得する。
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);

			if (!strcmp(p, "newmtl")){
				// 新規マテリアル名の宣言行です。
				material.push_back(MTLFileModelMaterial());
				// マテリアル名の取得
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setName(p);
			}
			else if (!strcmp(p, "Kd")){
				// ディフューズ色です。
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setKd(r, g, b);
			}
			else if (!strcmp(p, "Ks")){
				// スペキュラ色です
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setKs(r, g, b);
			}
			else if (!strcmp(p, "Ka")){
				// アンビエント色です
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setKa(r, g, b);
			}
			else if (!strcmp(p, "d")){
				// 透過度
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				float tr = strtof(p, &e);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setD(tr);
			}
			else if (!strcmp(p, "Ns")){
				char c = getUnsignedNumberStr(binary, p);
				int tr = strtol(p, &e, 0);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				material.back().setNs(tr);
			}
			else if (!strcmp(p, "map_Kd")){
				// テクスチャ名
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// 改行の必要がなくなった
					isGoNextLine = false;
				}
				std::ostringstream oss;
				oss << m_directoryName << "/" << p;
				material.back().setTextureName(oss.str().c_str());
			}

			if (isGoNextLine){
				goNextLine(binary);
			}
		}
	}

	// .objデータをロードする。
	void OBJModelFile::loadObjData(char* binary, std::vector<Vec3>& xyzList, std::vector<Vec3>& nomalList, std::vector<Vec2>& uvList, std::vector<std::vector<Vec3I>>& dataIndexList){
		char str[512];
		char error[128];
		char* p = str;
		char* e = error;
		int dataIndexIndex = 0;
		while (true){
			// 行の先頭文字によってループ内容を変更する。

			if (binary[0] == '\0'){
				// 終了。
				return;
			}

			// 先頭文字を取得
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);

			if (!strcmp(p, "v")){
				// 座標の読み込み
				Vec3 xyz;

				// x座標を取得しつつ読み飛ばし。
				goNextSpaceAndGetStr(binary, p);
				// 今一番高速と思われる方法で文字列を数値に変換。
				xyz.x = strtof(p, &e);

				// y座標を取得しつつ読み飛ばし。
				goNextSpaceAndGetStr(binary, p);
				xyz.y = strtof(p, &e);

				// z座標を取得しつつ読み飛ばし。
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				xyz.z = strtof(p, &e);

				xyzList.push_back(xyz);
			}
			else if (!strcmp(p, "vt")){
				// uvの読み込み
				Vec2 uv;
				// uの情報文字列を読み飛ばしつつ取得し、数値に変換。
				goNextSpaceAndGetStr(binary, p);
				uv.x = strtof(p, &e);

				// vの情報文字列を読み飛ばしつつ取得し、数値に変換。
				// どうやらOpenGL仕様がデフォルトのデータのようで、Directxのuvと上下が逆のようだ。
				// よって、vを反転している。
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				uv.y = 1.f - strtof(p, &e);

				uvList.push_back(uv);
			}
			else if (!strcmp(p, "vn")){
				// 法線の読み込み　（津山城データには法線がない模様。後日法線ありのモデルを読み込んだ際にデバッグの必要性あり。）
				Vec3 nomal;
				// x法線を読み飛ばしつつ変換、取得
				goNextSpaceAndGetStr(binary, p);
				nomal.x = strtof(p, &e);
				// y法線を読み飛ばしつつ変換、取得
				goNextSpaceAndGetStr(binary, p);
				nomal.y = strtof(p, &e);
				// y法線を読み飛ばしつつ変換、取得
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				nomal.z = strtof(p, &e);

				nomalList.push_back(nomal);
			}
			else if (!strcmp("f", p)){
				// 扱いやすいように、参照に入れておく。
				std::vector<Vec3I>& index = dataIndexList[dataIndexIndex];
				// この面の頂点の数だけ解析ループ。
				int size = index.size();
				for (int i = 0; i < size; i++){
					// 扱いやすいように、更に参照に入れておく。
					Vec3I dataIndex;
					// テクスチャデータ、法線データのあるなしに関わりデータの置き方が変わるのでややこしい。
					// まず、一つは必ず「ある」
					// 次の区切りまで読み飛ばしつつ解析。	
					char next = getUnsignedNumberStr(binary, p);
					dataIndex.x = strtol(p, &e, 0);
					--dataIndex.x;
					// 二つ目移行があれば解析。
					if (next == '/'){
						// 最後の文字がスラッシュだったので２つ目以降がある。

						// いきなりスラッシュがあるかどうか。
						if (binary[0] == '/'){
							// いきなりスラッシュがある。＝uv無しの、法線データありデータ。
							// スラッシュ飛ばして
							++binary;
							// 区切りまで読み飛ばしつつ解析
							next = getUnsignedNumberStr(binary, p);
							if (next == 0x0a){
								// 改行の必要がなくなった
								isGoNextLine = false;
							}
							dataIndex.z = strtol(p, &e, 0);
							--dataIndex.z;
							index[i].x = dataIndex.x;
							index[i].y = -1;
							index[i].z = dataIndex.z;
						}
						else{
							// スラッシュではなく数値がある。= 座標/uv/法線 or 座標/uv
							// 次の区切りまで読み飛ばしつつデータを取得、解析。
							next = getUnsignedNumberStr(binary, p);
							dataIndex.y = strtol(p, &e, 0);
							--dataIndex.y;
							if (next == '/'){
								// 最後に再びスラッシュがあった。= 座標/uv/法線
								next = getUnsignedNumberStr(binary, p);
								if (next == 0x0a){
									// 改行の必要がなくなった
									isGoNextLine = false;
								}
								dataIndex.z = strtol(p, &e, 0);
								--dataIndex.z;
								index[i] = dataIndex;
								if (dataIndexIndex == 426){
									int j = 0;
								}
							}
							else{
								if (next == 0x0a){
									// 改行の必要がなくなった
									isGoNextLine = false;
								}
								// スラッシュがなかった。= 座標/uv
								index[i].x = dataIndex.x;
								index[i].y = dataIndex.y;
								index[i].z = -1;
							}
						}
					}
					else{
						// ２つ目以降存在しない。＝座標のみ。
						index[i].x = dataIndex.x;
						index[i].y = -1;
						index[i].z = -1;
					}
				}
				// 面のカウントをインクリメント。
				++dataIndexIndex;
			}
			else if (binary[0] == '\0'){
				// データの終端まで来たので、解析を終了
				return;
			}
			// 改行する。
			if (isGoNextLine){
				goNextLine(binary);
			}
		}
	}

	// 次のスラッシュ'/'まで読み飛ばす。
	bool OBJModelFile::goNextSlash(char*& data){
		while (true){
			char c = *data++;
			if (c == '/'){
				return true;
			}
			else if (c == ' '){
				return false;
			}
		}
	}

	// その文字が、数字であるかどうか。
	bool OBJModelFile::isCharcterUnsignedNumber(char c) const{
		return (c >= '0' && c <= '9');
	}

	// faceデータ専用で、何角形か調べつつデータを読み進める。改行後まで進むことは無いことが保証される。
	int OBJModelFile::getCountFaceVertexNumByDataLine(char*& binary){
		// 今のポインタのシークから、行末までの符号なし整数値orスラッシュ以外の文字＝面数とし、
		// 更にその次の文字が数字でない場合、カウントを終了する。
		int count = 0;
		while (true){
			// 行末
			if (!(isCharcterUnsignedNumber(*binary) || *binary == '/')){
				// 頂点をカウントする。
				++count;
				// 更に次の文字が符号なし数字以外なら頂点カウントを終了。
				if (!isCharcterUnsignedNumber(binary[1])){
					break;
				}
			}
			// 次の文字へ
			++binary;
		}
		return count;
	}

	// 符号なし整数文字列を取得する。符号なし整数以外の文字が来るまでの長さの文字列とする。
	char OBJModelFile::getUnsignedNumberStr(char*& binary, char*& str){
		int count = 0;
		while (true){
			char c = *binary++;
			if (isCharcterUnsignedNumber(c)){
				str[count] = c;
			}
			else{
				str[count] = '\0';
				return c;
			}
			count++;
		}
	}

	// 名前などに使える文字以外がくるまで読み飛ばしつつ取得する。最後に来た文字を返す。
	char OBJModelFile::goNextBorderAndGetNameStr(char*& data, char*& str){
		int index = 0;
		while (true){
			char c = *data++;
			if (isCanUseNamingCharacter(c)){
				str[index] = c;
			}
			else{
				str[index] = '\0';
				return c;
			}
			++index;
		}
	}
	// 名前などにそのまま使用できる文字かどうかを判定する
	bool OBJModelFile::isCanUseNamingCharacter(char c){
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-' || c == '_' || c == '.';
	}
	// 符号なし浮動小数点に使える文字以外がくるまで読み飛ばしつつ取得する
	char OBJModelFile::goNextBorderAndGetUnsignedFloatStr(char*& binary, char*& str){
		int index = 0;
		while (true){
			char c = *binary++;
			if (isCharcterUnsignedNumber(c) || c == '.'){
				str[index] = c;
			}
			else{
				// 最後には終端を入れておく。
				str[index] = '\0';
				return c;
			}
			index++;
		}
	}

	// 次のスラッシュまで読み飛ばしつつそれまでの文字列を取得　スペースまでにスラッシュがなかった場合そこで終了しfalseを返す
	bool OBJModelFile::goNextSlashAndGetStr(char*& data, char*& str){
		int index = 0;
		while (true){
			char c = *data++;
			if (c == '/'){
				// 最後には終端を入れておく。
				str[index] = '\0';
				return true;
			}
			else if (c == ' '){
				str[index] = '\0';
				return false;
			}
			else{
				str[index] = c;
			}
			index++;
		}
	}

	// 頂点数、面数を数える
	void OBJModelFile::countDataXYZ_UV_Nomal_Face(char* data, int& xyz, int& uv, int& nomal, int& faceNum, int& vertBuf, int& indexBuf, int& groupNum, int& meshNum){
		// 例外処理
		char str[512];
		char* p = str;
		// 値を初期化
		//ASSERT(m_faceCount == -1 && m_vertexCount == -1 && "二回数えようとしています。");
		//m_faceCount = m_vertexCount = 0;
		xyz = uv = nomal = faceNum = vertBuf = indexBuf = groupNum = meshNum = 0;
		while (true){
			// 改行しつつ、先頭文字列によって頂点数と面数をカウントする。

			// データの最後に来たので終了。
			if (data[0] == '\0'){
				break;
			}

			// ループ終わりに改行するか？ ＆先頭文字列を取得。
			bool isGoNextLine = goNextSpaceAndGetStr(data, p);

			if (!strcmp("v", p)){
				// 座標情報
				++xyz;
			}
			else if (!strcmp("vt", p)){
				// uv情報
				++uv;
			}
			else if (!strcmp("vn", p)){
				// 法線情報
				++nomal;
			}
			else if (!strcmp("f", p)){
				// 面情報
				++faceNum;
				// 何角系か調べる。
				int count = getCountFaceVertexNumByDataLine(data);
				if (count == 3){
					// 三角形です。
					vertBuf += 3;
					indexBuf += 3;
				}
				else if (count == 4){
					// 四角形です。
					vertBuf += 4;
					indexBuf += 6;
				}
				else{
					HALT(未対応の形です);
				}
			}
			else if (!strcmp("g", p)){
				// グループ。
				++groupNum;
			}
			else if (!strcmp("usemtl", p)){
				// マテリアル名。＝メッシュの区切り。
				++meshNum;
			}

			if (isGoNextLine){
				// 改行の必要があるので改行する
				goNextLine(data);
			}
		}
	}

	// 次のスペースまで読み飛ばす。だが、改行を超えるのはこのデータの場合おかしいので改行でも止める。
	bool OBJModelFile::goNextSpace(char*& data){
		while (true){
			char c = *data++;
			if (c == ' '){
				return true;
			}
			else if (c == 0x0a){
				return false;
			}
		}
	}

	// 次のスペースまで読みとばしつつもそれまでの文字列を取得
	bool OBJModelFile::goNextSpaceAndGetStr(char*& data, char*& str){
		int index = 0;
		while (true){
			char c = *data++;
			if (c == ' '){
				// 最後には終端を入れておく。
				str[index] = '\0';
				return true;
			}
			else if (c == 0x0a){
				str[index] = '\0';
				return false;
			}
			else{
				str[index] = c;
			}
			index++;
		}
	}

	const std::vector<Vec3>& OBJModelFile::getPosition(){
		return m_destxyz;
	}
	const std::vector<unsigned>& OBJModelFile::getVertexIndex(){
		return m_destIndexBuf;
	}
	const std::vector<Vec3>& OBJModelFile::getNormal(){
		return m_destnomal;
	}
	const std::vector<Vec2>& OBJModelFile::getUV(){
		return m_destuv;
	}

	int OBJModelFile::vertexSize(){
		return m_vertexCount;
	}
	int OBJModelFile::vertexIndexSize(){
		return m_destIndexBuf.size();
	}


	const std::vector<std::string> OBJModelFile::getTextureName(){
		return m_textureNameList;
	}
	int OBJModelFile::getTextureCount(){
		return m_textureNameList.size();
	}

}