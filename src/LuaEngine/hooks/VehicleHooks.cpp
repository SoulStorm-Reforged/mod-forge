/*
 * Copyright (C) 2010 - 2016 Forge Lua Engine <http://emudevs.com/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ForgeTemplate.h"

#ifndef CLASSIC
#ifndef TBC

using namespace Hooks;

#define START_HOOK(EVENT) \
    if (!IsEnabled())\
        return;\
    auto key = EventKey<VehicleEvents>(EVENT);\
    if (!VehicleEventBindings->HasBindingsFor(key))\
        return;\
    LOCK_FORGE

void Forge::OnInstall(Vehicle* vehicle)
{
    START_HOOK(VEHICLE_EVENT_ON_INSTALL);
    Push(vehicle);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnUninstall(Vehicle* vehicle)
{
    START_HOOK(VEHICLE_EVENT_ON_UNINSTALL);
    Push(vehicle);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnInstallAccessory(Vehicle* vehicle, Creature* accessory)
{
    START_HOOK(VEHICLE_EVENT_ON_INSTALL_ACCESSORY);
    Push(vehicle);
    Push(accessory);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 seatId)
{
    START_HOOK(VEHICLE_EVENT_ON_ADD_PASSENGER);
    Push(vehicle);
    Push(passenger);
    Push(seatId);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnRemovePassenger(Vehicle* vehicle, Unit* passenger)
{
    START_HOOK(VEHICLE_EVENT_ON_REMOVE_PASSENGER);
    Push(vehicle);
    Push(passenger);
    CallAllFunctions(VehicleEventBindings, key);
}

#endif // CLASSIC
#endif // TBC
