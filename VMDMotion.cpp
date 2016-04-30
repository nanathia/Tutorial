#include "VMDMotion.h"
#include "Director.h"
#include "GyuDon.h"
#include "INCLUDES.h"
#include "MyFUNCTION.h"

// とりあ全Lerpで

VMDMotion::VMDMotion(const std::string& fileName, float speed)
{
	m_file = m_file->LoadFromFile(fileName.c_str());

	for (int i = 0; i < m_file->bone_frames.size(); i++){
		m_keyFrames[m_file->bone_frames[i].name][m_file->bone_frames[i].frame] = &m_file->bone_frames[i];
		if (m_file->bone_frames[i].frame > m_maxFrame){
			m_maxFrame = m_file->bone_frames[i].frame;
		}
	}

	m_OneFrameDelta = 1.f / (float)m_maxFrame * speed;
	m_speed = speed;
	m_frameCount = 0;
}


VMDMotion::~VMDMotion()
{
}

void VMDMotion::SetSpeed(float speed){
	m_speed = speed;
}

void VMDMotion::update(){
	//RenderFrameBone(boneName, out, m_ratio);
	m_ratio += Director::instance()->framework()->delta() * m_speed * 60.f;
	if ((int)m_ratio >= m_maxFrame){
		m_ratio = 0.f;
	}
}

void VMDMotion::RenderFrameBone(const std::string& boneName, D3DXMATRIX* out){
	if (m_keyFrames[boneName].size() == 0){
		D3DXMatrixIdentity(out);
		return;
	}
	else if (m_keyFrames[boneName].size() == 1){
		vmd::VmdBoneFrame* frame = m_keyFrames[boneName][0];
		D3DXMATRIX trance;
		D3DXQUATERNION* quat = reinterpret_cast<D3DXQUATERNION*>(frame->orientation);
		D3DXMATRIX rota;
		D3DXMatrixTranslation(&trance, frame->position[0], frame->position[1], frame->position[2]);
		D3DXMatrixRotationQuaternion(&rota, quat);
		*out = rota*trance;
		return;
	}
	std::map<int, vmd::VmdBoneFrame*>* bone = &m_keyFrames[boneName];
	auto frame = bone->upper_bound((int)m_ratio);
	vmd::VmdBoneFrame* _1 = 0;
	vmd::VmdBoneFrame* _2 = 0;
	float maxFrame_inKey = -1;
	if (bone->end() == frame){
		// 最後の補完だから最終と０
		_1 = bone->rbegin()->second;
		_2 = bone->begin()->second;
		maxFrame_inKey = float(m_maxFrame - _1->frame);
	}
	else{
		//if (frame->second->frame == m_ratio){ // 丁度
		//	D3DXMATRIX trance;
		//	D3DXQUATERNION* quat = reinterpret_cast<D3DXQUATERNION*>(frame->second->orientation);
		//	D3DXMATRIX rota;
		//	D3DXMatrixTranslation(&trance, frame->second->position[0], frame->second->position[1], frame->second->position[2]);
		//	D3DXMatrixRotationQuaternion(&rota, quat);
		//	*out = rota*trance;
		//	return;
		//}
		//else{
			// 普通の補完
			_2 = frame->second;
			_1 = (--frame)->second; // 前はかならずある、何故なら１移行で０キーが必ずあるから。
			maxFrame_inKey = _2->frame - _1->frame;
		//}
	}
	// ここまで来たなら補完
	float frame_inKey = m_ratio - _1->frame;
	float ratio = frame_inKey / maxFrame_inKey;
	D3DXVECTOR3 pos;
	D3DXQUATERNION rota;
	float ratioX, ratioY, ratioZ, ratioR;
	// ベジェ
	{
		ratioX = _1->bezieX.GetY(ratio);
		ratioY = _1->bezieY.GetY(ratio);
		ratioZ = _1->bezieZ.GetY(ratio);
		ratioR = _1->bezieR.GetY(ratio);
	}

	//D3DXVec3Lerp(&pos, reinterpret_cast<D3DXVECTOR3*>(_1->position), reinterpret_cast<D3DXVECTOR3*>(_2->position), ratio);
	pos.x = Lerp(_1->position[0], _2->position[0], ratioX);
	pos.y = Lerp(_1->position[1], _2->position[1], ratioY);
	pos.z = Lerp(_1->position[2], _2->position[2], ratioZ);
	D3DXQuaternionSlerp(&rota, reinterpret_cast<D3DXQUATERNION*>(_1->orientation), reinterpret_cast<D3DXQUATERNION*>(_2->orientation), ratioR);
	D3DXMATRIX rotaM;
	D3DXMatrixRotationQuaternion(&rotaM, &rota);
	D3DXMATRIX tranceM;
	D3DXMatrixTranslation(&tranceM, pos.x, pos.y, pos.z);
	*out = rotaM*tranceM;
}