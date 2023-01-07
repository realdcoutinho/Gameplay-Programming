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

bool Inventory::IsEmpty(IExamInterface* pInterface)
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


bool Inventory::HasSolt()
{
	for (auto check : m_ArrayCheck)
	{
		if (check == false)
			return true;
	}
	return false;
}

UINT Inventory::GetOpenSlot()
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

bool Inventory::AddItem(ItemInfo& item, EntityInfo& entity, IExamInterface* pInterface)
{
	switch (item.Type)
	{
		case eItemType::PISTOL:
		{
			return AddPistol(item, entity, pInterface);
		}
		break;
		case eItemType::FOOD:
		{
			return AddFood(item, entity, pInterface);
		}
		break;
		case eItemType::MEDKIT:
		{
			return AddMedKit(item, entity, pInterface);
		}
		break;
		case eItemType::SHOTGUN:
		{
			return AddShotGun(item, entity, pInterface);
		}
		break;
		case eItemType::GARBAGE:
		{
			pInterface->Item_Destroy(entity);
			return false;
		}
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
	
	//if (HasSolt())
	//{
	//	if (pInterface->Item_Grab(entity, item))
	//	{
	//		if (pInterface->Inventory_AddItem(slot, item))
	//		{
	//			m_InventoryArray[slot] = item;
	//			CloseSlot(slot);
	//			return true;
	//		}
	//	}
	//}
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
			
			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					//pInterface->Inventory_AddItem(slot, item);
					RemoveByItem(match, pInterface);
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
		else
		{
			pInterface->Item_Destroy(entity);
			std::cout << "Gun Was Not Added" << '\n';
			return false;
		}
	}
	else
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
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
	ItemInfo foodInfo{};
	foodInfo.Type = eItemType::FOOD;
	bool hasFood{ false };
	ItemInfo match{};
	UINT slot = GetOpenSlot();

	//if (HasSolt())
	//{
	//	if (pInterface->Item_Grab(entity, item))
	//	{
	//		if (pInterface->Inventory_AddItem(slot, item))
	//		{
	//			m_InventoryArray[slot] = item;
	//			CloseSlot(slot);
	//			return true;
	//		}
	//	}
	//}
	for (auto it : m_InventoryArray)
	{
		if (foodInfo.Type == it.Type)
		{
			hasFood = true;
			match = it;
			break;
		}
	}
	if (hasFood)
	{
		int currentFood = pInterface->Food_GetEnergy(match);
		int newFood = pInterface->Food_GetEnergy(item);

		if (newFood > currentFood)
		{

			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					RemoveByItem(match, pInterface);
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
		else
		{
			pInterface->Item_Destroy(entity);
			std::cout << "Food Was Not Added" << '\n';
			return false;
		}
	}
	else
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
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

	//if (HasSolt())
	//{
	//	if (pInterface->Item_Grab(entity, item))
	//	{
	//		if (pInterface->Inventory_AddItem(slot, item))
	//		{
	//			m_InventoryArray[slot] = item;
	//			CloseSlot(slot);
	//			return true;
	//		}
	//	}
	//}
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
		int currentHealth = pInterface->Medkit_GetHealth(match);
		int newHealth = pInterface->Medkit_GetHealth(item);

		if (newHealth > currentHealth)
		{

			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					RemoveByItem(match, pInterface);
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
		else
		{
			pInterface->Item_Destroy(entity);
			std::cout << "Food Was Not Added" << '\n';
			return false;
		}
	}
	else
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
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

	//if (HasSolt())
	//{
	//	if (pInterface->Item_Grab(entity, item))
	//	{
	//		if (pInterface->Inventory_AddItem(slot, item))
	//		{
	//			m_InventoryArray[slot] = item;
	//			CloseSlot(slot);
	//			return true;
	//		}
	//	}
	//}
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

			if (pInterface->Item_Grab(entity, item))
			{
				if (pInterface->Inventory_AddItem(slot, item))
				{
					RemoveByItem(match, pInterface);
					m_InventoryArray[slot] = item;
					CloseSlot(slot);
					return true;
				}
			}
		}
		else
		{
			pInterface->Item_Destroy(entity);
			std::cout << "Gun Was Not Added" << '\n';
			return false;
		}
	}
	else
	{
		if (pInterface->Item_Grab(entity, item))
		{
			if (pInterface->Inventory_AddItem(slot, item))
			{
				m_InventoryArray[slot] = item;
				CloseSlot(slot);
				return true;
			}
		}
	}
	return false;
}

void Inventory::RemoveByItem(ItemInfo& item, IExamInterface* pInterface)
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
	RemoveItemBySlot(slot, pInterface);
}

void Inventory::RemoveItemBySlot(UINT slot, IExamInterface* pInterface)
{
	ItemInfo emptyItemInfo{};
	emptyItemInfo.Type = eItemType::GARBAGE;
	m_InventoryArray[slot] = emptyItemInfo;
	m_ArrayCheck[slot] = false;
	pInterface->Inventory_RemoveItem(slot);
}

void Inventory::RemoveLast(EntityInfo& entity, IExamInterface* pInterface)
{
	ItemInfo emptyItemInfo{};
	emptyItemInfo.Type = eItemType::GARBAGE;
	UINT slot = m_MaxCapacity - 1;
	m_InventoryArray[slot] = emptyItemInfo;
	pInterface->Inventory_RemoveItem(slot);
}

bool Inventory::HasType(ItemInfo& compare)
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

bool Inventory::UseType(IExamInterface* pInterface, ItemInfo& compare)
{
	for (UINT slot{ 0 }; slot < m_MaxCapacity; ++slot)
	{
		if (m_InventoryArray[slot].Type == compare.Type)
		{
			pInterface->Inventory_UseItem(slot);
			return true;
		}
	}
	return false;

}