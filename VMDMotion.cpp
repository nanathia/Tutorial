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


void VMDMotion::UpdateIK() {
	//D3DXVECTOR3 localEffectorPos, localTargetPos;
	//for (unsigned int j = 0; j < ikData.iterations; ++j) {
	//	for (unsigned int i = 0; i < ikData.ik_child_bone_index.size(); ++i) {
	//		unsigned short attentionIdx = ikData.ik_child_bone_index[i];
	//		D3DXVECTOR3 effectorPos((*bones)[ikData.ik_target_bone_index].CalBoneMatML().m[3]);	// エフェクタ(ターゲットボーン)の位置
	//		D3DXVECTOR3 targetPos((*bones)[ikData.ik_bone_index].CalBoneMatML().m[3]);			// ターゲット(IKボーン)の位置
	//		D3DXMATRIX invCoord;																	// 注目ボーンのボーン行列の逆行列
	//		D3DXMatrixInverse(&invCoord, 0, &(*bones)[attentionIdx].CalBoneMatML());
	//		D3DXVec3TransformCoord(&localEffectorPos, &effectorPos, &invCoord);						// 注目ボーン基準に変換
	//		D3DXVec3TransformCoord(&localTargetPos, &targetPos, &invCoord);							// 注目ボーン基準に変換
	//		D3DXVECTOR3 localEffectorDir;															// エフェクタのローカル方向（注目ボーン基準）
	//		D3DXVec3Normalize(&localEffectorDir, &localEffectorPos);
	//		D3DXVECTOR3 localTargetDir;																// ターゲットのローカル方向（注目ボーン基準）
	//		D3DXVec3Normalize(&localTargetDir, &localTargetPos);
	//		if ((*bones)[attentionIdx].name.find("ひざ") != string::npos) {
	//			localEffectorDir = D3DXVECTOR3(0, localEffectorDir.y, localEffectorDir.z);
	//			D3DXVec3Normalize(&localEffectorDir, &localEffectorDir);
	//			localTargetDir = D3DXVECTOR3(0, localTargetDir.y, localTargetDir.z);
	//			D3DXVec3Normalize(&localTargetDir, &localTargetDir);
	//		}
	//		float p = D3DXVec3Dot(&localEffectorDir, &localTargetDir);
	//		if (p > 1 - 1.0e-8f) continue;	// 計算誤差により1を越えるとacos()が発散するので注意!
	//		float angle = acos(p);
	//		if (angle > 4 * ikData.control_weight) angle = 4.0f*ikData.control_weight;
	//		D3DXVECTOR3 axis;
	//		D3DXVec3Cross(&axis, &localEffectorDir, &localTargetDir);
	//		D3DXMATRIX rotation;
	//		D3DXMatrixRotationAxis(&rotation, &axis, angle);
	//		if ((*bones)[attentionIdx].name.find("ひざ") != string::npos) {
	//			D3DXMATRIX inv;
	//			D3DXMatrixInverse(&inv, 0, &(*bones)[attentionIdx].initMatBL);
	//			D3DXMATRIX def = rotation*(*bones)[attentionIdx].boneMatBL*inv;
	//			D3DXVECTOR3 t(0, 0, 1);
	//			D3DXVec3TransformCoord(&t, &t, &def);
	//			if (t.y < 0) D3DXMatrixRotationAxis(&rotation, &axis, -angle);
	//			// 膝ボーンがエフェクタ(ターゲットボーン)より近い時は回転量を追加する
	//			float l = D3DXVec3Length(&localTargetPos) / D3DXVec3Length(&localEffectorPos);
	//			if (fabs(angle) <= D3DX_PI / 2 && l < 1.0f) {
	//				static const float a = 0.5f;	// 追加量の比例係数
	//				float diff = acosf(l)*a;		// 追加量
	//				static const float diff_limit = D3DX_PI / 6;	// 追加量の制限
	//				if (diff > diff_limit) {
	//					diff = diff_limit;
	//				}
	//				if (fabs(angle) > 1.0e-6f) diff *= angle / fabs(angle);	// 符号合わせ
	//				angle += diff;
	//			}
	//		}
	//		(*bones)[attentionIdx].boneMatBL = rotation*(*bones)[attentionIdx].boneMatBL;
	//	}
	//	const float errToleranceSq = 0.000001f;
	//	if (D3DXVec3LengthSq(&(localEffectorPos - localTargetPos)) < errToleranceSq) {
	//		return;
	//	}
	//}
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
	pos.x = MyFunc::Lerp(_1->position[0], _2->position[0], ratioX);
	pos.y = MyFunc::Lerp(_1->position[1], _2->position[1], ratioY);
	pos.z = MyFunc::Lerp(_1->position[2], _2->position[2], ratioZ);
	D3DXQuaternionSlerp(&rota, reinterpret_cast<D3DXQUATERNION*>(_1->orientation), reinterpret_cast<D3DXQUATERNION*>(_2->orientation), ratioR);
	D3DXMATRIX rotaM;
	D3DXMatrixRotationQuaternion(&rotaM, &rota);
	D3DXMATRIX tranceM;
	D3DXMatrixTranslation(&tranceM, pos.x, pos.y, pos.z);
	*out = rotaM*tranceM;
}