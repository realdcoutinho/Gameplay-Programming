#pragma once
#include "Exam_HelperStructs.h"
#include <vector>
#include <iostream>

class IBaseInterface;
class IExamInterface;

using namespace std;
using namespace Elite;

class Inventory
{
public:
	Inventory();
	~Inventory();

	//adds
	bool AddItem(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);

	//checks
	bool IsEmpty(IExamInterface* pInterface);
	bool HasSolt();
	
	//uses
	bool HasType(ItemInfo& compare);
	bool UseType(IExamInterface* pInterface, ItemInfo& compare);

	//removes from blackboard
	bool RemoveEmpty(IExamInterface* pInterface);

private:

	vector<ItemInfo> m_Inventory;
	UINT m_MaxCapacity;
	ItemInfo m_InventoryArray[5]{};
	bool m_ArrayCheck[5]{ 0, 0, 0, 0, 0 };

	UINT GetOpenSlot();
	void CloseSlot(UINT slot);


	bool AddPistol(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddFood(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddMedKit(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddShotGun(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);

	//removes
	void RemoveByItem(ItemInfo& item, IExamInterface* pInterface);
	void RemoveItemBySlot(UINT slot, IExamInterface* pInterface);
	void RemoveLast(EntityInfo& entity, IExamInterface* pInterface);
};