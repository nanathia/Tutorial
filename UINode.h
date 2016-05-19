#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include "MyVector.h"

// 2D•`‰æ‚ÌÛ‚Ì•`‰æ’PˆÊ‚ği‚éƒNƒ‰ƒX‚Å‚·B

class UINode :
	public Ref
{
	Vec2 m_pos;
	Vec2 m_scale;
	float m_angle;

	int m_zOrder;
	Vec2 m_ancer;

public:
	UINode();
	virtual ~UINode();

	void SetPos(const Vec2& pos);
	void SetScale(const Vec2& scale);
	void SetAngle(const Vec2& angle);
	
	const Vec2& GetPos() const;
	const Vec2& GetScale() const;
	const Vec2& GetAncer() const;

};

