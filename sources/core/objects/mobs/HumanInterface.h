#pragma once

#include <QVector>

#include "core/objects/Object.h"
#include "representation/ViewInfo.h"

#include "Slot.h"
#include "core/SaveableOperators.h"

namespace kv
{
class Human;
class Item;

class HumanInterface2 : public Object
{
public:
    DECLARE_SAVEABLE(HumanInterface2, Object);
    REGISTER_CLASS_AS(HumanInterface2);
    HumanInterface2();

    void SetOwner(IdPtr<Human> human);

    void HandleClick(const QString& name);

    bool PickItem(IdPtr<Item> item);
    void DropItem();

    IdPtr<Item> GetItemInActiveHand();

    IdPtr<Item> GetItem(const QString& slot_name);
    void RemoveItem(const QString& slot_name);
    bool InsertItem(const QString& slot_name, IdPtr<Item> item);

    void Represent();

    void RemoveItem(IdPtr<Item> item);

    void AddOverlays();

    void UpdatePulling(bool is_pulling);
private:
    bool InsertItem(Slot* slot, IdPtr<Item> item);
    void RemoveItem(Slot* slot);

    Slot& GetSlot(const QString& slot_name);
    Slot& GetActiveHand();
    void ApplyActiveHandOnSlot(Slot* slot);

    void SwapHands();

    IdPtr<Human> KV_SAVEABLE(owner_);
    QVector<Slot> KV_SAVEABLE(slots_);
    bool KV_SAVEABLE(active_hand_);

    ViewInfo KV_SAVEABLE(pull_view_);
};
END_DECLARE(HumanInterface2)

}

