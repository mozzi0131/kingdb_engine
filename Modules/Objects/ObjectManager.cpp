
#include "ObjectManager.h"
#include <vector>

// for log
#include "KDBLog.h"

ObjectManager* ObjectManager::ObjectManagerInstance = nullptr;
Object* ObjectManager::Root = nullptr;

ObjectManager::ObjectManager() {
	Root = new Object();
	//ROOT�� ���� ����
	Root->setObjectGCState(Object::EObjectGCState::ROOT);
	KDB_LOG("Root Created");

	Root->ObjectId = ROOT_ID;
	EveryObjects.insert(std::pair<uint32_t, Object*>(ROOT_ID, Root));
	// ROOT_ID���� ������ ������Ʈ �߰��� ������ ++�Ѵ�.
	ObjectIdIssuer = ROOT_ID;
}

ObjectManager* ObjectManager::Get() {
	if (ObjectManagerInstance == nullptr)
	{
		ObjectManagerInstance = new ObjectManager();
	}
	
	return ObjectManagerInstance;
}

void ObjectManager::ClearManager() {
	ObjectManager* NowManager = Get();
	NowManager->clearManager();

	delete NowManager;

	ObjectManagerInstance = nullptr;
}

uint32_t ObjectManager::AddObjectToManager(Object* newObject) {
	ObjectIdIssuer++;

	EveryObjects.insert(std::pair<uint32_t, Object*>(ObjectIdIssuer, newObject));

	return ObjectIdIssuer;
}

void ObjectManager::RemoveFromObjectManager(Object* removingObject) {
	EveryObjects.erase(removingObject->GetObjectId());
}

void ObjectManager::AddObjectToRootChild(Object* targetObject) {
	targetObject->changeOuter(Root);
	targetObject->setObjectGCState(Object::EObjectGCState::ReferencedAndNotChecked);
}

void ObjectManager::GarbageCollectObjects() {
	// 1) ReferencedAndNotChecked�� ã��
	//		-> ���� ��� None�� ��ü���� ã�� �޸� ����(GC),
	//		   �׸��� ��� None���� ���¸� �ʱ�ȭ����
	//			(��Ʈ���� �����ϴ°�... �����... �ٷ� ��Ʈ�� �����ϴ� �ֵ��� ��������� �ϳ�)

	// 2) �ش� ������Ʈ�� Outer�� ������Ʈ���� �ؽ��ʿ��� ã�� ReferencedAndNotChecked�� ����, �ڱ� �ڽ��� ReferencedAndCheckedChildren�� ����

	std::map<uint32_t, Object*>::iterator checkStartObjectIter = findReferencedAndNotCheckedObject();
	if (checkStartObjectIter == EveryObjects.end()) {
		// gc ����. 
		std::map<uint32_t, Object*> leftObjects;
		std::vector<Object*> removingObjects;
		removingObjects.reserve(EveryObjects.size() - 1);
		for (auto iter = EveryObjects.begin(); iter != EveryObjects.end(); ++iter) {
			if ((*iter).second->isObjectGCState(Object::EObjectGCState::None)) {
				removingObjects.push_back((*iter).second);
			}
			else {
				leftObjects.insert(*iter);
			}
		}

		EveryObjects.swap(leftObjects);

		// removingObjects �޸� ����
		for (uint32_t i = 0; i< removingObjects.size(); i++) {
			KDB_LOG("object Id %d Removed bg GC", removingObjects[i]->GetObjectId());

			delete removingObjects[i];			
		}

		// root�� outer�� �ֵ��� ��� ReferencedAndNotChecked�� ����
		const std::set<Object*>& childrenSet = Root->GetChildren();

		for (auto iter = childrenSet.begin(); iter != childrenSet.end(); ++iter) {
			// for testing
			if (!(*iter)->isObjectGCState(Object::EObjectGCState::None) && !(*iter)->isObjectGCState(Object::EObjectGCState::ROOT)) {
				KDB_LOG("double referenced %d", (*iter)->GetObjectId());
			}

			(*iter)->setObjectGCState(Object::EObjectGCState::ReferencedAndNotChecked);
		}
	}
	else {
		// ���۷��� üũ ����
		constexpr int32_t MAX_REFERENCE_CHECK_ITEM_NUM = 100;
		int32_t refCheckCount = 0;

		for (checkStartObjectIter; (checkStartObjectIter != EveryObjects.end() && refCheckCount < MAX_REFERENCE_CHECK_ITEM_NUM); ++checkStartObjectIter) {
			Object* checkingObject = (*checkStartObjectIter).second;

			if (checkingObject->GetOuter() == nullptr) {
				continue;
			}

			const std::set<Object*> childrenSet = checkingObject->GetChildren();

			for (auto iter = childrenSet.begin(); iter != childrenSet.end(); ++iter) {
				// for testing
				if (!(*iter)->isObjectGCState(Object::EObjectGCState::None) && !(*iter)->isObjectGCState(Object::EObjectGCState::ROOT)) {
					KDB_LOG("double referenced %d", (*iter)->GetObjectId());
				}

				if ((*iter)->isObjectGCState(Object::EObjectGCState::None)) {
					(*iter)->setObjectGCState(Object::EObjectGCState::ReferencedAndNotChecked);
				}
			}

			checkingObject->setObjectGCState(Object::EObjectGCState::ReferencedAndCheckedChildren);
			refCheckCount++;
		}
	}

	KDB_LOG("GC Finished");
}

void ObjectManager::clearManager() {
	// ��ü ������Ʈ �޸� ����
	for (auto iter = EveryObjects.begin(); iter != EveryObjects.end(); iter++) {
		delete (iter->second);
	}

	EveryObjects.empty();
	Root = nullptr;

	//// hash bucket ��� Ȯ��
	//if (Root->GetChildrenNum() != 0)
	//{
	//	std::cout << "not all object cleared!!!";
	//}
}

std::map<uint32_t, Object*>::iterator ObjectManager::findReferencedAndNotCheckedObject() {
	std::map<uint32_t, Object*>::iterator iter = EveryObjects.begin();

	//std::iterator<Object*> a = EvetyObjects.begin();
	//std::_Tree<Object*>::iterator a = EvetyObjects.begin();

	for (iter; iter != EveryObjects.end(); ++iter) {
		if ((*iter).second->isObjectGCState(Object::EObjectGCState::ReferencedAndNotChecked)) {
			return iter;
		}
	}

	// ReferencedAndNotChecked�� ������Ʈ�� ���ٸ� gc�Ҷ���
	return iter;
}