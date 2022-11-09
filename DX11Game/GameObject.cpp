#include "GameObject.h"

GameObject::GameObject() {
	Init();
}


GameObject::~GameObject() {

}


void GameObject::Init() {
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	size = XMFLOAT3(1.0f, 1.0f, 1.0f);

	collRadius = 0.5f;
	collSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
}

void GameObject::Uninit() {

}

void GameObject::Update() {

}

void GameObject::Draw() {

}