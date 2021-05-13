#ifndef __MODULES_OBEJCTS_OBJECT_MANAGER_H__
#define __MODULES_OBEJCTS_OBJECT_MANAGER_H__

#include <stdint.h>
#include <map>



#include "Object.h"

////////////////////////////////////
//
// ������Ʈ ������ Outer�� �Բ� �༭ ObjectArr�� �߰��԰� ���ÿ� OuterMap�� �߰�
// 1�� ObjectId�� ������ RootObject �ʿ�.
//
//
//

class ObjectManager
{
private:
	ObjectManager();

private:
	static ObjectManager* ObjectManagerInstance;
	static Object* Root;

	static const uint32_t ROOT_ID = 1;

	// ���� �����ϴ� ��ü object�� ���Ե� map. key : objectId
	std::map<uint32_t, Object*> EveryObjects;

	uint32_t ObjectIdIssuer;

public:
	static ObjectManager* Get();
	static void ClearManager();

	// @ return : ObjectId
	uint32_t AddObjectToManager(Object* newObject);
	void RemoveFromObjectManager(Object* removingObject);

	void AddObjectToRootChild(Object* targetObject);

	void GarbageCollectObjects();

private:
	void clearManager();

	// ~for gc
	std::map<uint32_t, Object*>::iterator findReferencedAndNotCheckedObject();
	// ~for gc
};


#endif //__MODULES_OBEJCTS_OBJECT_MANAGER_H__