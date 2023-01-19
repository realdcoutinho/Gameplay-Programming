#include "stdafx.h"
#include "Inventory.h"
#include "IExamInterface.h"
#include "IExamInterface.h"
#include <iostream>

using namespace std;
using namespace Elite;

Inventory::Inventory() :
	m_MaxCapacity{5}
{
	std::cout << "Inventory Created" << '\n';
	ItemInfo emptyItemInfo{};
	emptyItemInfo.Type = eItemType::GARBAGE;
	m_InventoryArray[0] = emptyItemInfo;
	m_InventoryArray[1] = emptyItemInfo;
	m_InventoryArray[2] = emptyItemInfo;
	m_InventoryArray[3] = emptyItemInfo;
	m_InventoryArray[4] = emptyItemInfo;
}

Inventory::~Inventory()
{
	std::cout << "Inventory Deleted" << '\n';
}

bool Inventory::AddItem(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	switch (item.Type)
	{

	
	case eItemType::FOOD:
		if (AddFood(item, entity, pInterface))
			return true;
		break;


	case eItemType::MEDKIT:
		if (AddMedKit(item, entity, pInterface))
			return true;
		break;


	case eItemType::SHOTGUN:
		if (AddShotGun(item, entity, pInterface))
			return true;
	break;


	case eItemType::PISTOL:
		if (AddPistol(item, entity, pInterface))
			return true;
		break;

	case eItemType::GARBAGE:
		//doesnt really add the garbage, only if he has a slot
		//if so, removes right away
		//explanation in the function itself
		if (AddGarbage(item, entity, pInterface))
			return true;
		break;
	}
}

bool Inventory::AddPistol(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo pistol{};
	pistol.Type = eItemType::PISTOL;
	bool hasGun{ false };
	ItemInfo match{};
	UINT slot = GetOpenSlot();
	
	for (auto it : m_InventoryArray)
	{
		if (pistol.Type == it.Type)
		{
			hasGun = true;
			match = it;
			break;
		}
	}
	if (hasGun)
	{
		int currentAmmo = pInterface->Weapon_GetAmmo(match);
		int newAmmo = pInterface->Weapon_GetAmmo(item);

		if (newAmmo > currentAmmo)
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS PISTOL" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE AMMO" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						return true;
					}
				}
				
			}
			if (pInterface->Item_Grab(entity, item))
			{
				RemoveByItem(match, pInterface);
				slot = GetOpenSlot();
				if (pInterface->Inventory_AddItem(slot, item))
				{
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS PISTOL" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE AMMO" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, REMOVED CURRENT" << '\n';
					return true;
				}
			}
		}
		else
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS PISTOL" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS AMMO" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVES SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						return true;
					}
				}
			}
			else
			{
				if (pInterface->Item_Destroy(entity))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS PISTOL" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS AMMO" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW DESTROYED, KEPT CURRENT" << '\n';
					return false;
				}
				return false;
			}
		}
	}
	if (HasAnyGun())
	{
		if (HasSolt())
		{
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE ANY GUN" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>PISTOL ADDED" << '\n';
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}

		if (pInterface->Item_Destroy(entity))
		{
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE ANY GUN" << '\n';
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>PISTOL DESTROYED" << '\n';
			return false;
		}
		return false;
	}
	if (HasSolt())
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO GUN" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>PISTOL ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	if (RemoveRepeated(pInterface))
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO GUN" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE  NO SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>PISTOL ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	return false;
}

bool Inventory::AddFood(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo food{};
	food.Type = eItemType::FOOD;
	bool hasFood{ false };
	ItemInfo match{};
	UINT slot = GetOpenSlot();
	for (auto it : m_InventoryArray)
	{
		if (food.Type == it.Type)
		{
			hasFood = true;
			match = it;
			break;
		}
	}

	if (hasFood)
	{
		int currentReward = pInterface->Food_GetEnergy(match);
		int newReward = pInterface->Food_GetEnergy(item);

		if (newReward > currentReward)
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS FOOD" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE ENERGY" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						return true;
					}
				}

			}
			if (pInterface->Item_Grab(entity, item))
			{
				//pInterface->Inventory_UseItem(slot);
				RemoveByItem(match, pInterface);
				slot = GetOpenSlot();
				if (pInterface->Inventory_AddItem(slot, item))
				{
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS FOOD" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE ENERGY" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, USED CURRENT" << '\n';
					return true;
				}
			}
		}
		else
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS FOOD" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS ENERGY" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVES SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						return true;
					}
				}
			}
			else
			{
				if (pInterface->Item_Destroy(entity))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS FOOD" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS ENERGY" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW DESTROYED, KEPT CURRENT" << '\n';
					return false;
				}
				return false;
			}
		}
	}
	if (HasSolt())
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO FOOD" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>FOOD ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	else
	{
		if (RemoveRepeated(pInterface))
		{
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO FOOD" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE  NO SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>FOOD ADDED" << '\n';
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
	}

	return false;
}

bool Inventory::AddMedKit(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo MedkitInfo{};
	MedkitInfo.Type = eItemType::MEDKIT;
	bool hasMedkit{ false };
	ItemInfo match{};
	UINT slot = GetOpenSlot();

	for (auto it : m_InventoryArray)
	{
		if (MedkitInfo.Type == it.Type)
		{
			hasMedkit = true;
			match = it;
			break;
		}
	}

	if (hasMedkit)
	{
		int currentReward = pInterface->Medkit_GetHealth(match);
		int newReward = pInterface->Medkit_GetHealth(item);

		if (newReward > currentReward)
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS MEDKIT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE HEALTH" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						return true;
					}
				}

			}
			if (pInterface->Item_Grab(entity, item))
			{
				RemoveByItem(match, pInterface);
				slot = GetOpenSlot();
				if (pInterface->Inventory_AddItem(slot, item))
				{
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS MEDKIT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE HEALTH" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, USED CURRENT" << '\n';
					return true;
				}
			}
		}
		else
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS MEDKIT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS HEALTH" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVES SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						return true;
					}
				}
			}
			else
			{
				if (pInterface->Item_Destroy(entity))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS MEDKIT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS HEALTH" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW DESTROYED, KEPT CURRENT" << '\n';
					return false;
				}
				return false;
			}
		}
	}
	if (HasSolt())
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO MEDKIT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>MEDKIT ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	else
	{
		if (RemoveRepeated(pInterface))
		{
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO MEDKIT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE  NO SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>MEDKIT ADDED" << '\n';
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
	}
	return false;
}

bool Inventory::AddShotGun(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo shotGun{};
	shotGun.Type = eItemType::SHOTGUN;
	bool hasShotgun{ false };
	ItemInfo match{};
	UINT slot = GetOpenSlot();



	if (item.Type != eItemType::SHOTGUN)
	{
		return false;
	}
	for (auto it : m_InventoryArray)
	{
		if (shotGun.Type == it.Type)
		{
			hasShotgun = true;
			match = it;
			break;
		}
	}
	if (hasShotgun)
	{
		int currentAmmo = pInterface->Weapon_GetAmmo(match);
		int newAmmo = pInterface->Weapon_GetAmmo(item);

		if (newAmmo > currentAmmo)
		{

			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SHOTGUN" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE AMMO" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						return true;
					}
				}
			}
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					RemoveByItem(match, pInterface);
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SHOTGUN" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS MORE AMMO" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, REMOVED CURRENT" << '\n';
					return true;
				}
			}
		}
		else
		{
			if (HasSolt())
			{
				if (pInterface->Item_Grab(entity, item))
				{
					if (pInterface->Inventory_AddItem(slot, item))
					{
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SHOTGUN" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS AMMO" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVES SLOT" << '\n';
						std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW ADDED, KEPT CURRENT" << '\n';
						m_InventoryArray[slot] = item;
						CloseSlot(slot);
						return true;
					}
				}
			}
			else
			{
				if (pInterface->Item_Destroy(entity))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAS SHOTGUN" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW HAS LESS AMMO" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>NEW DESTROYED, KEPT CURRENT" << '\n';
					return false;
				}
				return false;
			}
		}
	}

	if (HasAnyGun())
	{
		if (HasSolt())
		{
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE ANY GUN" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>SHOTGUN ADDED" << '\n';
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
		if (pInterface->Item_Destroy(entity))
		{
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE ANY GUN" << '\n';
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>DOES NOT HAVE SLOT" << '\n';
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>SHOTGUN DESTROYED" << '\n';
			return false;
		}
		return false;
	}
	if (HasSolt())
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO GUN" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>SHOTGUN ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	if (RemoveRepeated(pInterface))
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE NO GUN" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>HAVE  NO SLOT" << '\n';
				std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>SHOTGUN ADDED" << '\n';
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	return false;
}

bool Inventory::AddGarbage(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{


	if (item.Type != eItemType::GARBAGE) //IT NEEDS THIS CHECK. I CANNOT EXPLAIN THIS
		return false;

	//To increase the points:
	//The Agent will only pick the garbage if it has an open slot
	UINT slot = GetOpenSlot(); // gets the slot location thats free if any
	if (HasSolt()) //checks
	{
		if (pInterface->Item_Grab(entity, item)) //grabs
		{
			if (pInterface->Inventory_AddItem(slot, item)) //adds
			{
				//and removes right away, increase the score byb two point
				//also, it does not update the information on my costumized inventory
				//neither the bool or the items one;

				//if (item.Type != eItemType::GARBAGE) //IT NEEDS THIS CHECK
				//	return false;
				
				pInterface->Inventory_RemoveItem(slot);
				return true;
			}
		}
	}
	//if he doesnt have an open slot
	pInterface->Item_Destroy(entity); //destroys the garbage
	return false;
}

bool Inventory::RemoveByItem(ItemInfo& item, IExamInterface* pInterface)
{
	UINT slot{};
	for (UINT index{}; index < m_MaxCapacity - 1; ++index)
	{
		if (m_InventoryArray[index].Type == item.Type)
		{
			slot = index;
			break;
		}
	}
	return RemoveItemBySlot(slot, pInterface);
}

bool Inventory::RemoveItemBySlot(UINT slot, IExamInterface* pInterface)
{
	ItemInfo emptyItemInfo{};
	emptyItemInfo.Type = eItemType::GARBAGE;
	m_InventoryArray[slot] = emptyItemInfo;
	m_ArrayCheck[slot] = false;
	pInterface->Inventory_GetItem(slot, emptyItemInfo);
	if (emptyItemInfo.Type == eItemType::PISTOL)
	{
		return pInterface->Inventory_RemoveItem(slot);
	}
	if (emptyItemInfo.Type == eItemType::SHOTGUN)
	{
		return pInterface->Inventory_RemoveItem(slot);
	}
	//pInterface->Inventory_UseItem(slot);
	return pInterface->Inventory_RemoveItem(slot);
}

bool Inventory::RemoveLast(EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo emptyItemInfo{};
	emptyItemInfo.Type = eItemType::GARBAGE;
	UINT slot = m_MaxCapacity - 1;
	m_InventoryArray[slot] = emptyItemInfo;
	return pInterface->Inventory_RemoveItem(slot);
}

bool Inventory::HasType(ItemInfo& compare) const
{
	for (auto item : m_InventoryArray)
	{
		if (item.Type == compare.Type)
		{
			return true;
		}
	}
	return false;
}

bool Inventory::HasAnyGun() const
{
	ItemInfo pistol{};
	pistol.Type = eItemType::PISTOL;
	ItemInfo shotgun{};
	shotgun.Type = eItemType::SHOTGUN;

	for (auto item : m_InventoryArray)
	{
		if (item.Type == pistol.Type)
		{
			return true;
		}
		if (item.Type == shotgun.Type)
		{
			return true;
		}
	}
	return false; 
}

bool Inventory::RemoveRepeated(IExamInterface* pInterface)
{
	UINT maxMedKit{ 2 };
	UINT maxFood{ 3 };

	UINT nrMedKit{};
	UINT nrFood{};



	UINT maxPistol{ 2 };
	UINT maxShotgun{ 2 };
	UINT nrPistol{ 0 };
	UINT nrShotgun{ 0 };


	ItemInfo medkit{};
	medkit.Type = eItemType::MEDKIT;
	ItemInfo food{};
	food.Type = eItemType::FOOD;
	ItemInfo pistol{};
	pistol.Type = eItemType::PISTOL;
	ItemInfo shotgun{};
	shotgun.Type = eItemType::SHOTGUN;



	for (auto item : m_InventoryArray)
	{
		if (item.Type == eItemType::MEDKIT)
		{
			++nrMedKit;
		}
		if (item.Type == eItemType::FOOD)
		{
			++nrFood;
		}
		if (item.Type == eItemType::PISTOL)
		{
			++nrMedKit;
		}
		if (item.Type == eItemType::SHOTGUN)
		{
			++nrFood;
		}
	}
	if (nrPistol >= maxPistol)
	{
		return RemoveByItem(pistol, pInterface);
	}
	if (nrShotgun >= maxShotgun)
	{
		return RemoveByItem(shotgun, pInterface);
	}
	if (nrFood >= maxFood)
	{
		return UseType(pInterface, food);
	}
	if (nrMedKit >= maxMedKit)
	{
		return UseType(pInterface, medkit);
	}

	return false;
}

bool Inventory::UseType(IExamInterface* pInterface, ItemInfo& compare)
{
	for (UINT slot{ 0 }; slot < m_MaxCapacity; ++slot)
	{
		if (m_InventoryArray[slot].Type == compare.Type)
		{
			if (pInterface->Inventory_UseItem(slot))
			{
				return true;
			}
		}
	}
	return false;

}

bool Inventory::HasSolt() const
{
	for (auto check : m_ArrayCheck)
	{
		if (check == false)
			return true;
	}
	return false;
}

UINT Inventory::GetOpenSlot() const
{
	for (UINT position{ 0 }; position < m_MaxCapacity; ++position)
	{
		if (m_ArrayCheck[position] == false)
		{
			return position;
		}
	}
}

void Inventory::CloseSlot(UINT slot)
{
	m_ArrayCheck[slot] = true;
}

bool Inventory::IsEmpty(IExamInterface* pInterface) const
{
	for (auto it : m_InventoryArray)
	{
		switch (it.Type)
		{
		case eItemType::PISTOL:
		{
			if (pInterface->Weapon_GetAmmo(it) == 0)
			{
				return true;
			}
		}
		break;
		case eItemType::FOOD:
		{
			if (pInterface->Food_GetEnergy(it) == 0)
			{
				return true;
			}
		}
		break;
		case eItemType::MEDKIT:
		{
			if (pInterface->Medkit_GetHealth(it) == 0)
			{
				return true;
			}
		}
		break;
		case eItemType::SHOTGUN:
		{
			if (pInterface->Weapon_GetAmmo(it) == 0)
			{
				return true;
			}
		}
		break;
		}
	}
	return false;
}

bool Inventory::RemoveEmpty(IExamInterface* pInterface)
{
	for (UINT position{ 0 }; position < m_MaxCapacity - 1; ++position)
	{
		auto it = m_InventoryArray[position];
		switch (it.Type)
		{
		case eItemType::PISTOL:
		{
			if (pInterface->Weapon_GetAmmo(it) == 0)
			{
				pInterface->Inventory_RemoveItem(position);
				RemoveByItem(it, pInterface);
				return true;
			}
		}
		break;
		case eItemType::FOOD:
		{
			if (pInterface->Food_GetEnergy(it) == 0)
			{
				pInterface->Inventory_RemoveItem(position);
				RemoveByItem(it, pInterface);
				return true;
			}
		}
		break;
		case eItemType::MEDKIT:
		{
			if (pInterface->Medkit_GetHealth(it) == 0)
			{
				pInterface->Inventory_RemoveItem(position);
				RemoveByItem(it, pInterface);
				return true;
			}
		}
		break;
		case eItemType::SHOTGUN:
		{
			if (pInterface->Weapon_GetAmmo(it) == 0)
			{
				pInterface->Inventory_RemoveItem(position);
				RemoveByItem(it, pInterface);
				return true;
			}
		}
		break;
		}
	}

	return false;
}

