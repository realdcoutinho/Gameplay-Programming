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
	Inventory(const Inventory& other) = delete; //copy constructor
	Inventory& operator=(const Inventory& other) = delete; // assignment operator
	Inventory(Inventory&& other) = delete; // move constructor
	Inventory& operator=(Inventory&& other) = delete; // move assignment operator

	//adds
	bool AddItem(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);

	//checks
	bool IsEmpty(IExamInterface* pInterface) const;
	bool HasType(ItemInfo& compare) const;

	//uses
	bool UseType(IExamInterface* pInterface, ItemInfo& compare);

	//removes from blackboard
	bool RemoveEmpty(IExamInterface* pInterface);

private:

	//members
	UINT m_MaxCapacity;
	ItemInfo m_InventoryArray[5]{};
	bool m_ArrayCheck[5]{ 0, 0, 0, 0, 0 };

	UINT GetOpenSlot() const;
	void CloseSlot(UINT slot);

	//cheks
	bool HasAnyGun() const;
	bool HasSolt() const;

	//Add Specific Items
	bool AddPistol(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddFood(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddMedKit(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddShotGun(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);
	bool AddGarbage(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface);

	//Removes
	bool RemoveByItem(ItemInfo& item, IExamInterface* pInterface);
	bool RemoveItemBySlot(UINT slot, IExamInterface* pInterface);
	bool RemoveRepeated(IExamInterface* pInterface);
	bool RemoveLast(EntityInfo& entity, IExamInterface* pInterface);
};