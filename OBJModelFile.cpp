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
		// �f�[�^�ǂݍ���
		OBJDataLoadAndSetting();
		int i = 0;
	}

	// ���_����ǂ�
	void OBJModelFile::vertexRead(char*& data){
	}

	// ���̍s�܂œǂݔ�΂�
	void OBJModelFile::goNextLine(char*& data){
		while (true){
			if (*data++ == 0x0a){
				return;
			}
		}
	}

	// .mtl�t�@�C���Ɋւ��鏈��
	void OBJModelFile::MTLDataLoadAndSetting(){
		// .mtl�t�@�C���̓ǂݍ��݂ł�
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
			// �t�@�C���I�[��t����B�t�@�C���I�[�ɗ������ɁA��̓��[�v���~�߂Ȃ���΃��������[�N���邽�߁B
			binary[binarySize] = 0x0a;
			binary[binarySize + 1] = '\0';
		}

		// �܂��A�}�e���A���̐��𐔂���B
		int materialNum;
		countMaterialByBinaryData(binary, materialNum);

		// ���b�V���f�[�^�̔z����m�ۂ���B
		m_materialList.reserve(materialNum);

		// �}�e���A�����Z�b�g����
		setMaterialByBinaryData(binary, m_materialList);

		// �t�@�C���f�[�^�͕K�v�Ȃ��Ȃ����̂ŉ���B
		SAFE_DELETE_ARRAY(binary);
	}

	// .obj�t�@�C���Ɋւ��鏈��
	void OBJModelFile::OBJDataLoadAndSetting(){
		{
			// .obj�t�@�C���̓ǂݍ��݂ł��B
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
				// �t�@�C���I�[��t����B�t�@�C���I�[�ɗ������ɁA��̓��[�v���~�߂Ȃ���΃��������[�N���邽�߁B
				binary[binarySize] = 0x0a;
				binary[binarySize + 1] = '\0';
			}

			// �K�v�ȗv�f�̐��𐔂��ĕۑ����Ă����B
			int xyzNum, nomalNum, uvNum, faceNum, vertBufNum, IndexBufNum, groupNum, meshNum;
			countDataXYZ_UV_Nomal_Face(binary, xyzNum, uvNum, nomalNum, faceNum, vertBufNum, IndexBufNum, groupNum, meshNum);
			// �`��̎��ׂ̈ɁA�O�p�`�̐����v��B�C���f�N�X�o�b�t�@�̐�/3�B
			m_primitiveCount = IndexBufNum / 3;
			// �R�Ŋ���؂�Ȃ��Ƃ��������B
			ASSERT(!(IndexBufNum % 3));
			// �������A���_�̐���ۑ�����
			m_vertexCount = vertBufNum;
			// �O���[�v���X�g�̔z����m�ۂ���B
			m_groupList.reserve(groupNum);
			// ���b�V�����X�g�̔z����m�ۂ���B
			m_meshList.reserve(meshNum);

			// �ʖ��̒��_�����L�^����B
			std::vector<std::vector<Vec3I>> dataIndexList;
			dataIndexList.reserve(faceNum);
			countVertexForFace(binary, dataIndexList, m_groupList, m_meshList);

			// �f�[�^����A������f�[�^�𔲂��o�����߂̃o�b�t�@��p�ӂ���B
			std::vector<Vec3> xyz;
			xyz.reserve(xyzNum);
			std::vector<Vec3> normal;
			normal.reserve(nomalNum);
			std::vector<Vec2> uv;
			uv.reserve(uvNum);

			// �f�[�^����A�F�X�Ƃ𔲂��o���B
			loadObjData(binary, xyz, normal, uv, dataIndexList);

			// �����ŁA���̃f�[�^�͕s�v�ƂȂ�̂ŉ���B
			SAFE_DELETE_ARRAY(binary);

			// ���_�o�b�t�@�E�C���f�N�X�o�b�t�@�̍쐬
			m_destxyz.clear();
			m_destnomal.clear();
			m_destuv.clear();
			m_destIndexBuf.clear();
			m_destxyz.reserve(vertBufNum);
			m_destnomal.reserve(vertBufNum);
			m_destuv.reserve(vertBufNum);
			m_destIndexBuf.reserve(IndexBufNum);

			// ���_�o�b�t�@�E�C���f�N�X�o�b�t�@���e���|�����o�b�t�@�ɓǂݍ��ށB
			getVertexAndIndexBuffer(xyz, uv, normal, m_destxyz, m_destuv, m_destnomal, dataIndexList, m_destIndexBuf);

			// MTL��ǂݍ��ݙp��ɂ��UV�덷���C������B
			MTLDataLoadAndSetting();
			// �ŏI�����B
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

	// �ŏI�����B
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
			// �e�N�X�`���؂�ւ��́A�o���邾�����Ȃ��悤�ɕ��בւ���
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

	// ���_�o�b�t�@�ƃC���f�N�X�o�b�t�@���擾����B
	void OBJModelFile::getVertexAndIndexBuffer(
		std::vector<Vec3>& srcXYZ, std::vector<Vec2>& srcUV, std::vector<Vec3>& srcNomal,
		std::vector<Vec3>& destXYZ, std::vector<Vec2>& destUV, std::vector<Vec3>& destNomal,
		std::vector<std::vector<Vec3I>>& dataIndexData,
		std::vector<unsigned>& destIndexBuf)
	{
		// �쐬�������_�̐���ʖ��ɉ��Z���Ă䂭�B
		int vertexCountAll = 0;
		// �ʂ̐��������[�v���񂷁B
		for (auto srcIndex : dataIndexData){
			int vertexSize = srcIndex.size();
			// �@�����v�Z���邩�ǂ����̃t���O
			bool isNeedCreateNomal = false;
			// �܂��́A���_�o�b�t�@���쐬����B���_�o�b�t�@�̍쐬�Ɋւ��ẮA�ʂ̒��_���͊֗^���Ȃ��B
			for (int i = 0; i < vertexSize; i++){
				// .obj�f�[�^���炻�̂܂ܔ����o�����f�[�^�̃C���f�N�X�B���W�Auv�A�@���̏��B
				Vec3I& i2 = srcIndex[i];
				// ���W�͕K�����݂���B
				destXYZ.push_back(srcXYZ[i2.x]);
				// uv�͑��݂��Ȃ��\��������B
				if (i2.y == -1){
					// uv�͑��݂��܂���B
					destUV.push_back(0.f);
				}
				else{
					// uv�����݂��܂��B
					destUV.push_back(srcUV[i2.y]);
				}
				// �@�������݂��Ȃ��\��������B
				if (i2.z == -1){
					// �@�������݂��Ȃ��B�i���ƂŃ|���S���̌�������v�Z����j
					isNeedCreateNomal = true;
				}
				else{
					// �@�������݂���B
					ASSERT(!isNeedCreateNomal);
					destNomal.push_back(srcNomal[i2.z]);
				}
			}
			// ���_�o�b�t�@�쐬�����B
			// �@���v�Z�̕K�v������ꍇ�v�Z�B
			if (isNeedCreateNomal){
				// �|���S�����擾
				Vec3* poligon = &destXYZ[destXYZ.size() - vertexSize];
				// �v�Z�Ɏg�p����Q�x�N�g�����v�Z�B�O�p�`���낤�ƂS�p�`���낤�ƕς��Ȃ��B
				Vec3 v1 = poligon[1] - poligon[0];
				Vec3 v2 = poligon[2] - poligon[0];
				// �O�ς��擾�B
				Vec3 cross;
				D3DXVec3Cross(cross.getD3DVector(), v1.getD3DVector_c(), v2.getD3DVector_c());
				// ���K���B
				D3DXVec3Normalize(cross.getD3DVector(), cross.getD3DVector_c());
				// �@���Ƃ��ēK�p�B
				for (int i = 0; i < vertexSize; i++){
					destNomal.push_back(cross);
				}
			}
			// �C���f�N�X�o�b�t�@�̓K�p�B
			if (vertexSize == 3){
				// �R�ʂȂ�R�C���f�N�X�B
				destIndexBuf.push_back(vertexCountAll);
				destIndexBuf.push_back(vertexCountAll + 1);
				destIndexBuf.push_back(vertexCountAll + 2);
				vertexCountAll += 3;
			}
			else if (vertexSize == 4){
				// 4�ʂȂ�U�C���f�N�X
				destIndexBuf.push_back(vertexCountAll);
				destIndexBuf.push_back(vertexCountAll + 1);
				destIndexBuf.push_back(vertexCountAll + 2);
				destIndexBuf.push_back(vertexCountAll + 0);
				destIndexBuf.push_back(vertexCountAll + 2);
				destIndexBuf.push_back(vertexCountAll + 3);
				vertexCountAll += 4;
			}
			else{
				HALT(���Ή��̒��_��);
			}
		}
	}

	// �ʖ��̒��_�����L�^����B����ƁA�O���[�v���̏����擾����B
	void OBJModelFile::countVertexForFace(char* binary, std::vector<std::vector<Vec3I>>& dataIndexList, std::vector<OBJFileModelGroup>& group, std::vector<OBJFileModelMesh>& mesh){
		char str[512];
		char* p = str;
		// ���݂܂łɃv���O�������ʂ������_�̐��̍��v
		int runedVertexNum = 0;
		// ���݂Ƀv���O�������ʂ����C���f�N�X�o�b�t�@�̐��̍��v�B
		int runedIndexNum = 0;
		while (true){
			// �s�̐擪�����ɂ���ă��[�v���e��ύX����B

			if (binary[0] == '\0'){
				if (group.size()){
					// �O���[�v�����݂���B
					// �Ō�̃O���[�v�����B
					group.back().setVertexSize(runedVertexNum - group.back().getVertexStart());
					group.back().setIndexSize(runedIndexNum - group.back().getIndexStart());
				}
				// ���b�V��������Ε���B
				if (mesh.size()){
					mesh.back().setVertexSize(runedVertexNum - mesh.back().getVertexStart());
					mesh.back().setIndexSize(runedIndexNum - mesh.back().getIndexStart());
				}
				return;
			}

			// �擪�������擾
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);
			if (!strcmp("f", p)){
				// �ʏ��B�ʂ̒��_�����L�^����B
				// ���p�n�����ׂ�B
				int count = getCountFaceVertexNumByDataLine(binary);
				if (count == 3){
					// �O�p�`�ł��B
					dataIndexList.push_back(std::vector<Vec3I>());
					dataIndexList.back().resize(3);
					// �v���O�������ʂ������_�A�C���f�N�X�̍��v�����Z
					runedVertexNum += 3;
					runedIndexNum += 3;
				}
				else if (count == 4){
					// �l�p�`�ł��B
					dataIndexList.push_back(std::vector<Vec3I>());
					dataIndexList.back().resize(4);
					// �v���O�������ʂ������_�A�C���f�N�X�̍��v�����Z
					runedVertexNum += 4;
					runedIndexNum += 6;
				}
				else{
					HALT(���Ή��̌`�ł�);
				}
			}
			else if (!strcmp(p, "g")){
				// �O���[�v���L�^�B

				// ���̃O���[�v�̃C���X�^���X���A�O���[�v���X�g�̖����ɐ�������B
				group.push_back(OBJFileModelGroup());
				// �O���[�v�Ƃ��Ă܂Ƃ߂����v�́A�����炪���_�̂��̃O���[�v�̊J�n�n�_�ƂȂ�B
				group.back().setVertexStart(runedVertexNum);
				// ��L�Ɠ��l�̎����C���f�N�X���ɂ��K�p�B
				group.back().setIndexStart(runedIndexNum);

				if (group.size() >= 2){
					// �O�̃O���[�v�����݂���B
					OBJFileModelGroup& prev = group[group.size() - 2];
					// �O�̃O���[�v���������ꍇ�A
					// ���݂̒ʂ������_���������̃O���[�v�̐錾���������ꏊ�̒��_�����A
					// ���̃O���[�v�̒��_�T�C�Y�ƂȂ�B
					prev.setVertexSize(runedVertexNum - prev.getVertexStart());
					prev.setIndexSize(runedIndexNum - prev.getIndexStart());
				}

				// �O���[�v����ǂݔ�΂��擾�B
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// ���̍s�̐擪�܂ŗ��Ă��܂����̂ŁA�X�ɉ��s����K�v�������Ȃ����B
					isGoNextLine = false;
				}
				// �O���[�v�̃C���X�^���X�ɖ��O��ݒ�
				group.back().setGroupName(p);

			}
			else if (!strcmp(p, "usemtl")){
				// ���b�V���̎擾�B

				// ���b�V�����X�g�̖����ɃC���X�^���X�𐶐�
				mesh.push_back(OBJFileModelMesh());

				// �O���[�v���X�g�Ɠ����悤�ɓK�p����B�����̃u���b�N�̓O���[�v�̏ꍇ�ƑS����������
				mesh.back().setVertexStart(runedVertexNum);
				mesh.back().setIndexStart(runedIndexNum);
				if (mesh.size() >= 2){
					OBJFileModelMesh& prev = mesh[mesh.size() - 2];
					prev.setVertexSize(runedVertexNum - prev.getVertexStart());
					prev.setIndexSize(runedIndexNum - prev.getIndexStart());
				}

				// �}�e���A�������A�ǂݔ�΂��擾����
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// ���̍s�̐擪�܂ŗ��Ă��܂����̂ŁA�Ō�ɉ��s����K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				// ���b�V�����X�g�̈�Ԗ����̃��b�V���ɁA�}�e���A������K�p�B
				mesh.back().setMaterialName(p);
			}

			// ���s
			if (isGoNextLine){
				goNextLine(binary);
			}
		}
	}

	// �}�e���A���̐��𐔂���B
	void OBJModelFile::countMaterialByBinaryData(char* data, int& materialNum){
		char str[256];
		char* p = str;
		char error[128];
		char* e = error;
		materialNum = 0;

		while (true){

			if (*data == '\0'){
				// �f�[�^�̏I�[�܂ŗ����̂ŏI���B
				return;
			}

			// �擪��������擾����B
			bool isGoNextLine = goNextSpaceAndGetStr(data, p);

			if (!strcmp(p, "newmtl")){
				// �V�K�}�e���A�����̐錾�s�ł��B
				++materialNum;
			}

			if (isGoNextLine){
				goNextLine(data);
			}
		}
	}

	// �}�e���A�����Z�b�g����B
	void OBJModelFile::setMaterialByBinaryData(char* binary, std::vector<MTLFileModelMaterial>& material){
		char str[256];
		char* p = str;
		char error[128];
		char* e = error;

		while (true){

			if (*binary == '\0'){
				// �f�[�^�̏I�[�܂ŗ����̂ŏI���B
				return;
			}

			// �擪��������擾����B
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);

			if (!strcmp(p, "newmtl")){
				// �V�K�}�e���A�����̐錾�s�ł��B
				material.push_back(MTLFileModelMaterial());
				// �}�e���A�����̎擾
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setName(p);
			}
			else if (!strcmp(p, "Kd")){
				// �f�B�t���[�Y�F�ł��B
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setKd(r, g, b);
			}
			else if (!strcmp(p, "Ks")){
				// �X�y�L�����F�ł�
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setKs(r, g, b);
			}
			else if (!strcmp(p, "Ka")){
				// �A���r�G���g�F�ł�
				float r, g, b;
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				r = strtof(p, &e);
				goNextBorderAndGetUnsignedFloatStr(binary, p);
				g = strtof(p, &e);
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				b = strtof(p, &e);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setKa(r, g, b);
			}
			else if (!strcmp(p, "d")){
				// ���ߓx
				char c = goNextBorderAndGetUnsignedFloatStr(binary, p);
				float tr = strtof(p, &e);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setD(tr);
			}
			else if (!strcmp(p, "Ns")){
				char c = getUnsignedNumberStr(binary, p);
				int tr = strtol(p, &e, 0);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
					isGoNextLine = false;
				}
				material.back().setNs(tr);
			}
			else if (!strcmp(p, "map_Kd")){
				// �e�N�X�`����
				char c = goNextBorderAndGetNameStr(binary, p);
				if (c == 0x0a){
					// ���s�̕K�v���Ȃ��Ȃ���
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

	// .obj�f�[�^�����[�h����B
	void OBJModelFile::loadObjData(char* binary, std::vector<Vec3>& xyzList, std::vector<Vec3>& nomalList, std::vector<Vec2>& uvList, std::vector<std::vector<Vec3I>>& dataIndexList){
		char str[512];
		char error[128];
		char* p = str;
		char* e = error;
		int dataIndexIndex = 0;
		while (true){
			// �s�̐擪�����ɂ���ă��[�v���e��ύX����B

			if (binary[0] == '\0'){
				// �I���B
				return;
			}

			// �擪�������擾
			bool isGoNextLine = goNextSpaceAndGetStr(binary, p);

			if (!strcmp(p, "v")){
				// ���W�̓ǂݍ���
				Vec3 xyz;

				// x���W���擾���ǂݔ�΂��B
				goNextSpaceAndGetStr(binary, p);
				// ����ԍ����Ǝv������@�ŕ�����𐔒l�ɕϊ��B
				xyz.x = strtof(p, &e);

				// y���W���擾���ǂݔ�΂��B
				goNextSpaceAndGetStr(binary, p);
				xyz.y = strtof(p, &e);

				// z���W���擾���ǂݔ�΂��B
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				xyz.z = strtof(p, &e);

				xyzList.push_back(xyz);
			}
			else if (!strcmp(p, "vt")){
				// uv�̓ǂݍ���
				Vec2 uv;
				// u�̏�񕶎����ǂݔ�΂��擾���A���l�ɕϊ��B
				goNextSpaceAndGetStr(binary, p);
				uv.x = strtof(p, &e);

				// v�̏�񕶎����ǂݔ�΂��擾���A���l�ɕϊ��B
				// �ǂ����OpenGL�d�l���f�t�H���g�̃f�[�^�̂悤�ŁADirectx��uv�Ə㉺���t�̂悤���B
				// ����āAv�𔽓]���Ă���B
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				uv.y = 1.f - strtof(p, &e);

				uvList.push_back(uv);
			}
			else if (!strcmp(p, "vn")){
				// �@���̓ǂݍ��݁@�i�ÎR��f�[�^�ɂ͖@�����Ȃ��͗l�B����@������̃��f����ǂݍ��񂾍ۂɃf�o�b�O�̕K�v������B�j
				Vec3 nomal;
				// x�@����ǂݔ�΂��ϊ��A�擾
				goNextSpaceAndGetStr(binary, p);
				nomal.x = strtof(p, &e);
				// y�@����ǂݔ�΂��ϊ��A�擾
				goNextSpaceAndGetStr(binary, p);
				nomal.y = strtof(p, &e);
				// y�@����ǂݔ�΂��ϊ��A�擾
				isGoNextLine = goNextSpaceAndGetStr(binary, p);
				nomal.z = strtof(p, &e);

				nomalList.push_back(nomal);
			}
			else if (!strcmp("f", p)){
				// �����₷���悤�ɁA�Q�Ƃɓ���Ă����B
				std::vector<Vec3I>& index = dataIndexList[dataIndexIndex];
				// ���̖ʂ̒��_�̐�������̓��[�v�B
				int size = index.size();
				for (int i = 0; i < size; i++){
					// �����₷���悤�ɁA�X�ɎQ�Ƃɓ���Ă����B
					Vec3I dataIndex;
					// �e�N�X�`���f�[�^�A�@���f�[�^�̂���Ȃ��Ɋւ��f�[�^�̒u�������ς��̂ł�₱�����B
					// �܂��A��͕K���u����v
					// ���̋�؂�܂œǂݔ�΂���́B	
					char next = getUnsignedNumberStr(binary, p);
					dataIndex.x = strtol(p, &e, 0);
					--dataIndex.x;
					// ��ڈڍs������Ή�́B
					if (next == '/'){
						// �Ō�̕������X���b�V���������̂łQ�ڈȍ~������B

						// �����Ȃ�X���b�V�������邩�ǂ����B
						if (binary[0] == '/'){
							// �����Ȃ�X���b�V��������B��uv�����́A�@���f�[�^����f�[�^�B
							// �X���b�V����΂���
							++binary;
							// ��؂�܂œǂݔ�΂����
							next = getUnsignedNumberStr(binary, p);
							if (next == 0x0a){
								// ���s�̕K�v���Ȃ��Ȃ���
								isGoNextLine = false;
							}
							dataIndex.z = strtol(p, &e, 0);
							--dataIndex.z;
							index[i].x = dataIndex.x;
							index[i].y = -1;
							index[i].z = dataIndex.z;
						}
						else{
							// �X���b�V���ł͂Ȃ����l������B= ���W/uv/�@�� or ���W/uv
							// ���̋�؂�܂œǂݔ�΂��f�[�^���擾�A��́B
							next = getUnsignedNumberStr(binary, p);
							dataIndex.y = strtol(p, &e, 0);
							--dataIndex.y;
							if (next == '/'){
								// �Ō�ɍĂуX���b�V�����������B= ���W/uv/�@��
								next = getUnsignedNumberStr(binary, p);
								if (next == 0x0a){
									// ���s�̕K�v���Ȃ��Ȃ���
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
									// ���s�̕K�v���Ȃ��Ȃ���
									isGoNextLine = false;
								}
								// �X���b�V�����Ȃ������B= ���W/uv
								index[i].x = dataIndex.x;
								index[i].y = dataIndex.y;
								index[i].z = -1;
							}
						}
					}
					else{
						// �Q�ڈȍ~���݂��Ȃ��B�����W�̂݁B
						index[i].x = dataIndex.x;
						index[i].y = -1;
						index[i].z = -1;
					}
				}
				// �ʂ̃J�E���g���C���N�������g�B
				++dataIndexIndex;
			}
			else if (binary[0] == '\0'){
				// �f�[�^�̏I�[�܂ŗ����̂ŁA��͂��I��
				return;
			}
			// ���s����B
			if (isGoNextLine){
				goNextLine(binary);
			}
		}
	}

	// ���̃X���b�V��'/'�܂œǂݔ�΂��B
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

	// ���̕������A�����ł��邩�ǂ����B
	bool OBJModelFile::isCharcterUnsignedNumber(char c) const{
		return (c >= '0' && c <= '9');
	}

	// face�f�[�^��p�ŁA���p�`�����ׂf�[�^��ǂݐi�߂�B���s��܂Ői�ނ��Ƃ͖������Ƃ��ۏ؂����B
	int OBJModelFile::getCountFaceVertexNumByDataLine(char*& binary){
		// ���̃|�C���^�̃V�[�N����A�s���܂ł̕����Ȃ������lor�X���b�V���ȊO�̕������ʐ��Ƃ��A
		// �X�ɂ��̎��̕����������łȂ��ꍇ�A�J�E���g���I������B
		int count = 0;
		while (true){
			// �s��
			if (!(isCharcterUnsignedNumber(*binary) || *binary == '/')){
				// ���_���J�E���g����B
				++count;
				// �X�Ɏ��̕����������Ȃ������ȊO�Ȃ璸�_�J�E���g���I���B
				if (!isCharcterUnsignedNumber(binary[1])){
					break;
				}
			}
			// ���̕�����
			++binary;
		}
		return count;
	}

	// �����Ȃ�������������擾����B�����Ȃ������ȊO�̕���������܂ł̒����̕�����Ƃ���B
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

	// ���O�ȂǂɎg���镶���ȊO������܂œǂݔ�΂��擾����B�Ō�ɗ���������Ԃ��B
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
	// ���O�Ȃǂɂ��̂܂܎g�p�ł��镶�����ǂ����𔻒肷��
	bool OBJModelFile::isCanUseNamingCharacter(char c){
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-' || c == '_' || c == '.';
	}
	// �����Ȃ����������_�Ɏg���镶���ȊO������܂œǂݔ�΂��擾����
	char OBJModelFile::goNextBorderAndGetUnsignedFloatStr(char*& binary, char*& str){
		int index = 0;
		while (true){
			char c = *binary++;
			if (isCharcterUnsignedNumber(c) || c == '.'){
				str[index] = c;
			}
			else{
				// �Ō�ɂ͏I�[�����Ă����B
				str[index] = '\0';
				return c;
			}
			index++;
		}
	}

	// ���̃X���b�V���܂œǂݔ�΂�����܂ł̕�������擾�@�X�y�[�X�܂łɃX���b�V�����Ȃ������ꍇ�����ŏI����false��Ԃ�
	bool OBJModelFile::goNextSlashAndGetStr(char*& data, char*& str){
		int index = 0;
		while (true){
			char c = *data++;
			if (c == '/'){
				// �Ō�ɂ͏I�[�����Ă����B
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

	// ���_���A�ʐ��𐔂���
	void OBJModelFile::countDataXYZ_UV_Nomal_Face(char* data, int& xyz, int& uv, int& nomal, int& faceNum, int& vertBuf, int& indexBuf, int& groupNum, int& meshNum){
		// ��O����
		char str[512];
		char* p = str;
		// �l��������
		//ASSERT(m_faceCount == -1 && m_vertexCount == -1 && "��񐔂��悤�Ƃ��Ă��܂��B");
		//m_faceCount = m_vertexCount = 0;
		xyz = uv = nomal = faceNum = vertBuf = indexBuf = groupNum = meshNum = 0;
		while (true){
			// ���s���A�擪������ɂ���Ē��_���Ɩʐ����J�E���g����B

			// �f�[�^�̍Ō�ɗ����̂ŏI���B
			if (data[0] == '\0'){
				break;
			}

			// ���[�v�I���ɉ��s���邩�H ���擪��������擾�B
			bool isGoNextLine = goNextSpaceAndGetStr(data, p);

			if (!strcmp("v", p)){
				// ���W���
				++xyz;
			}
			else if (!strcmp("vt", p)){
				// uv���
				++uv;
			}
			else if (!strcmp("vn", p)){
				// �@�����
				++nomal;
			}
			else if (!strcmp("f", p)){
				// �ʏ��
				++faceNum;
				// ���p�n�����ׂ�B
				int count = getCountFaceVertexNumByDataLine(data);
				if (count == 3){
					// �O�p�`�ł��B
					vertBuf += 3;
					indexBuf += 3;
				}
				else if (count == 4){
					// �l�p�`�ł��B
					vertBuf += 4;
					indexBuf += 6;
				}
				else{
					HALT(���Ή��̌`�ł�);
				}
			}
			else if (!strcmp("g", p)){
				// �O���[�v�B
				++groupNum;
			}
			else if (!strcmp("usemtl", p)){
				// �}�e���A�����B�����b�V���̋�؂�B
				++meshNum;
			}

			if (isGoNextLine){
				// ���s�̕K�v������̂ŉ��s����
				goNextLine(data);
			}
		}
	}

	// ���̃X�y�[�X�܂œǂݔ�΂��B�����A���s�𒴂���̂͂��̃f�[�^�̏ꍇ���������̂ŉ��s�ł��~�߂�B
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

	// ���̃X�y�[�X�܂œǂ݂Ƃ΂�������܂ł̕�������擾
	bool OBJModelFile::goNextSpaceAndGetStr(char*& data, char*& str){
		int index = 0;
		while (true){
			char c = *data++;
			if (c == ' '){
				// �Ō�ɂ͏I�[�����Ă����B
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