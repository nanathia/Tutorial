#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include "MyVector.h"


// UI�̃X�v���C�g���N���X�ł��B
class UISprite : public Ref{
	Vec2 m_pos; // ������W
	Vec2 m_ancerPoint; // ���S�_�䗦
	Vec2 m_size; // �傫��
	int m_Zorder; // �`�揇��
public:
	UISprite();
	~UISprite();
	// get
	const Vec2& GetPos() const;
	const Vec2& GetSize() const;
	const Vec2& GetAncerPoint() const;
	int GetZorder() const;
	// set
	void SetPos(const Vec2& pos);
	void SetSize(const Vec2 size);
	void SetAncerPoint(const Vec2& size);
	void SetZorder(int order);
};

// 2DUI��`�悷�邽�߂̃Z�b�g���ڂ�S�ĊǗ�����N���X�ł��B
// �����Ɋe�X�v���C�g����n�����Ƃŕ`�悳��܂��B

class UIDrawSetter :
	public Ref
{
	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	ID3D11Buffer* m_pVertexBuffer = NULL;
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11BlendState* m_hpBlendState = NULL;
	struct CONSTANT{
		D3DXMATRIX mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 Diffuse = D3DXVECTOR4(1, 0, 0, 1); //�g�U����(�F�j	
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 UV;
	};

	void init();
	void exit();

public:
	UIDrawSetter();
	~UIDrawSetter();

	void draw();

};

