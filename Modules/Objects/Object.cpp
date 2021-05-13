
#include "Object.h"
#include "ObjectManager.h"

#include "KDBLog.h"

Object::Object() {
	reset();
}

Object::~Object() {
	KDB_LOG("object Id %d destroyed", ObjectId);
	reset();
}

void Object::reset() {
	ObjectId = 0;
	Outer = nullptr;
	GcState = EObjectGCState::None;
}

void Object::setObjectGCState(EObjectGCState newState) {
	// root�� ��� ���� ���� �Ұ���
	if (GcState != EObjectGCState::ROOT)
	{
		GcState = newState;
	}
}

bool Object::isObjectGCState(EObjectGCState gcState) const {
	return (GcState == gcState);
}

void Object::addChild(Object* newChild) {
	Children.insert(newChild);
}

void Object::removeChild(Object* targetChild) {
	Children.erase(targetChild);
}

void Object::clearOuter() {
	// ���� outer
	if (Outer)
	{
		Outer->removeChild(this);
	}
	Outer = nullptr;
}

void Object::changeOuter(Object* newOuter) {
	// ���� outer
	if (Outer) {
		Outer->removeChild(this);
	}
	
	SetOuter(newOuter);
}

void Object::clearChildren() {
	for (auto iter = Children.begin(); iter != Children.end(); ++iter) {
		(*iter)->Outer = nullptr;
	}

	Children.empty();
}

Object* Object::CreateObject(Object* outer) {
	Object* createdObject = new Object();

	uint32_t createdObjectId = ObjectManager::Get()->AddObjectToManager(createdObject);
	KDB_LOG("object Id %d Created", createdObjectId);

	createdObject->ObjectId = createdObjectId;

	if (outer != nullptr) {
		outer->addChild(createdObject);
	}
	createdObject->SetOuter(outer);
	return createdObject;
}

void Object::SetOuter(Object* newOuter) {
	// todo : �� outer�� �θ�(recursive)�� �ڽ��� �ִ��� Ȯ�� �ʿ�
	Outer = newOuter;
}

void Object::Destroy() {
	// �ڽ��� Outer�� Children���� �ڽ� ����
	if (Outer != nullptr) {
		Outer->removeChild(this);
	}

	// �ڽ��� child�� outer ����

	clearChildren();
	ObjectManager::Get()->RemoveFromObjectManager(this);

	delete this;
}