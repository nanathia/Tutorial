#pragma once
#include "Ref.h"
#include "lib/MikuMikuFormats/Vmd.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <vector>
#include <map>

namespace pmx{
	class PmxBone;
}

class VMDMotion :
	public Ref
{
	float m_ratio = 0.f;
	float m_OneFrameDelta = 0.f;
	float m_speed = 1.f;
	int m_frameCount = 0;
	int m_maxFrame = -1;

	std::unique_ptr<vmd::VmdMotion> m_file;
	std::string m_fileName = "";

	std::map<std::string, std::map<int, vmd::VmdBoneFrame*/*キーフレーム*/>/*ボーン名*/> m_keyFrames;

public:
	void SetSpeed(float speed);

	// 
	void update();
	// 特定フレームでの指定ボーンの差分を書き出し。
	void RenderFrameBone(const std::string& boneName, D3DXMATRIX* out);

	VMDMotion(const std::string& fileName, float speed = 1.f);
	~VMDMotion();
};

