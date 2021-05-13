#ifndef __MODULES_OBEJCTS_OBJECT_H__
#define __MODULES_OBEJCTS_OBJECT_H__


#include <stdint.h>
#include <set>

// Base Object Class

class Object
{
	friend class ObjectManager;

	enum class EObjectGCState {
		/*should be GCed*/ None = 0,
		/*Referenced from other object, do not check child*/ ReferencedAndNotChecked = 1,
		/*Referenced from other object, checked all child*/ ReferencedAndCheckedChildren = 2,
		/*Root Object should not be garbage collected and change to other state. */ ROOT = 3,

		// GC (Tri-color marking): https://www.slideshare.net/QooJuice/ue4-garbage-collection-2
		// 1) ��� None���� �ʱ�ȭ
		// 2) Root���� ���� ������ ��ü�� ReferencedAndNotChecked�� ����
		// 3) ReferencedAndNotChecked���� �����ϴ� ��ü�� ReferencedAndNotChecked�� ����,
		//	  ReferencedAndNotChecked�� ReferencedAndCheckedChildren�� ����
		// 4) ReferencedAndNotChecked�� ��ü�� �������� ������ None�� ��ü���� �޸� ����
	};
private:
	// Object::CreateObject() ���� ���� ����
	Object();
	~Object();

	void reset();
	
	void setObjectGCState(EObjectGCState newState);
	bool isObjectGCState(EObjectGCState gcState) const;

	void addChild(Object* newChild);
	void removeChild(Object* targetChild);
	void clearOuter();
	void changeOuter(Object* newOuter);
	void clearChildren();
	
private:
	// ������Ʈ ���� Id
	uint32_t ObjectId = 0;

	// GC�� ���� ������Ʈ ������Ʈ
	EObjectGCState GcState = EObjectGCState::None;

	// ���� ������Ʈ�� ����Ű�� �θ� Object
	Object* Outer = nullptr;

	// �ڽ��� Outer�� ������ �ڽ� ������Ʈ
	std::set<Object*> Children;

public:
	
	uint32_t GetObjectId() const { return ObjectId; }
	Object* GetOuter() const { return Outer; }
	void SetOuter(Object* newOuter);

	const std::set<Object*>& GetChildren() { return Children; }
	int32_t GetChildrenNum() { return Children.size(); }

	void Destroy();

	bool operator==(const Object& other) const {
		return (other.ObjectId == ObjectId);
	}

	bool operator!=(const Object& other) const {
		return !(*this == other);
	}

	static Object* CreateObject(Object* outer);
};

#endif //__MODULES_OBEJCTS_OBJECT_H__