#include "GameObject.h"

GameObject::GameObject() {
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_size = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_radius = 5.0f;
}


GameObject::~GameObject() {

}
