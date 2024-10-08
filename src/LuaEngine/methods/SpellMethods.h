/*
* Copyright (C) 2010 - 2016 Forge Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef SPELLMETHODS_H
#define SPELLMETHODS_H

/***
 * An instance of a spell, created when the spell is cast by a [Unit].
 *
 * Inherits all methods from: none
 */
namespace LuaSpell
{
    /**
     * Returns `true` if the [Spell] is automatically repeating, `false` otherwise.
     *
     * @return bool isAutoRepeating
     */
    int IsAutoRepeat(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->IsAutoRepeat());
        return 1;
    }

    /**
     * Returns the [Unit] that casted the [Spell].
     *
     * @return [Unit] caster
     */
    int GetCaster(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->GetCaster());
        return 1;
    }

    /**
     * Returns the cast time of the [Spell].
     *
     * @return int32 castTime
     */
    int GetCastTime(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->GetCastTime());
        return 1;
    }

    /**
     * Returns the entry ID of the [Spell].
     *
     * @return uint32 entryId
     */
    int GetEntry(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->m_spellInfo->Id);
        return 1;
    }

    /**
     * Returns the power cost of the [Spell].
     *
     * @return uint32 powerCost
     */
    int GetPowerCost(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->GetPowerCost());
        return 1;
    }

    /**
     * Returns the reagents needed for the [Spell].
     *
     * @return table reagents : a table containing the [ItemTemplate]s and amount of reagents needed for the [Spell]
    */
    int GetReagentCost(lua_State* L, Spell* spell)
    {
        auto spellInfo = spell->GetSpellInfo();
        auto reagents = spellInfo->Reagent;
        auto reagentCounts = spellInfo->ReagentCount;
        lua_newtable(L);
        for (auto i = 0; i < MAX_SPELL_REAGENTS; ++i)
        {
            if (reagents[i] <= 0)
                continue;
            auto reagent = eObjectMgr->GetItemTemplate(reagents[i]);
            auto count = reagentCounts[i];
            Forge::Push(L, reagent);
            Forge::Push(L, count);
            lua_settable(L, -3);
        }
        return 1;
    }

    /**
     * Returns the spell duration of the [Spell].
     *
     * @return int32 duration
     */
    int GetDuration(lua_State* L, Spell* spell)
    {
#if defined TRINITY || AZEROTHCORE
        Forge::Push(L, spell->GetSpellInfo()->GetDuration());
#else
        Forge::Push(L, GetSpellDuration(spell->m_spellInfo));
#endif
        return 1;
    }

    /**
     * Returns the target destination coordinates of the [Spell].
     *
     * @return float x : x coordinate of the [Spell]
     * @return float y : y coordinate of the [Spell]
     * @return float z : z coordinate of the [Spell]
     */
    int GetTargetDest(lua_State* L, Spell* spell)
    {
#if defined TRINITY || AZEROTHCORE
        if (!spell->m_targets.HasDst())
            return 3;
        float x, y, z;
        spell->m_targets.GetDstPos()->GetPosition(x, y, z);
#else
        if (!(spell->m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION))
            return 3;
        float x, y, z;
        spell->m_targets.getDestination(x, y, z);
#endif
        Forge::Push(L, x);
        Forge::Push(L, y);
        Forge::Push(L, z);
        return 3;
    }

    /**
     * Returns the target [Object] of the [Spell].
     *
     * The target can be any of the following [Object] types:
     * - [Player]
     * - [Creature]
     * - [GameObject]
     * - [Item]
     * - [Corpse]
     *
     * @return [Object] target
     */
    int GetTarget(lua_State* L, Spell* spell)
    {
#if defined TRINITY || AZEROTHCORE
        if (GameObject* target = spell->m_targets.GetGOTarget())
            Forge::Push(L, target);
        else if (Item* target = spell->m_targets.GetItemTarget())
            Forge::Push(L, target);
        else if (Corpse* target = spell->m_targets.GetCorpseTarget())
            Forge::Push(L, target);
        else if (Unit* target = spell->m_targets.GetUnitTarget())
            Forge::Push(L, target);
        else if (WorldObject* target = spell->m_targets.GetObjectTarget())
            Forge::Push(L, target);
#else
        if (GameObject* target = spell->m_targets.getGOTarget())
            Forge::Push(L, target);
        else if (Item* target = spell->m_targets.getItemTarget())
            Forge::Push(L, target);
        else if (Corpse* target = spell->GetCaster()->GetMap()->GetCorpse(spell->m_targets.getCorpseTargetGuid()))
            Forge::Push(L, target);
        else if (Unit* target = spell->m_targets.getUnitTarget())
            Forge::Push(L, target);
#endif
        return 1;
    }

    /**
     * Sets the [Spell] to automatically repeat.
     *
     * @param bool repeat : set variable to 'true' for spell to automatically repeat
     */
    int SetAutoRepeat(lua_State* L, Spell* spell)
    {
        bool repeat = Forge::CHECKVAL<bool>(L, 2);
        spell->SetAutoRepeat(repeat);
        return 0;
    }

    /**
     * Casts the [Spell].
     *
     * @param bool skipCheck = false : skips initial checks to see if the [Spell] can be casted or not, this is optional
     */
    int Cast(lua_State* L, Spell* spell)
    {
        bool skipCheck = Forge::CHECKVAL<bool>(L, 2, false);
        spell->cast(skipCheck);
        return 0;
    }

    /**
     * Cancels the [Spell].
     */
    int Cancel(lua_State* /*L*/, Spell* spell)
    {
        spell->cancel();
        return 0;
    }

    /**
     * Finishes the [Spell].
     */
    int Finish(lua_State* /*L*/, Spell* spell)
    {
        spell->finish();
        return 0;
    }

    /**
     * Get the [SpellInfo].
     *
     * @return [SpellInfo] spell_info
     */
    int GetSpellInfo(lua_State* L, Spell* spell)
    {
        Forge::Push(L, spell->GetSpellInfo());
        return 1;
    }

    /**
     * Send cast result for the [Spell]
     *
     * enum SpellCastResult
     * {
     *    SPELL_FAILED_SUCCESS = 0,
     *    SPELL_FAILED_AFFECTING_COMBAT = 1,
     *    SPELL_FAILED_ALREADY_AT_FULL_HEALTH = 2,
     *    SPELL_FAILED_ALREADY_AT_FULL_MANA = 3,
     *    SPELL_FAILED_ALREADY_AT_FULL_POWER = 4,
     *    SPELL_FAILED_ALREADY_BEING_TAMED = 5,
     *    SPELL_FAILED_ALREADY_HAVE_CHARM = 6,
     *    SPELL_FAILED_ALREADY_HAVE_SUMMON = 7,
     *    SPELL_FAILED_ALREADY_OPEN = 8,
     *    SPELL_FAILED_AURA_BOUNCED = 9,
     *    SPELL_FAILED_AUTOTRACK_INTERRUPTED = 10,
     *    SPELL_FAILED_BAD_IMPLICIT_TARGETS = 11,
     *    SPELL_FAILED_BAD_TARGETS = 12,
     *    SPELL_FAILED_CANT_BE_CHARMED = 13,
     *    SPELL_FAILED_CANT_BE_DISENCHANTED = 14,
     *    SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL = 15,
     *    SPELL_FAILED_CANT_BE_MILLED = 16,
     *    SPELL_FAILED_CANT_BE_PROSPECTED = 17,
     *    SPELL_FAILED_CANT_CAST_ON_TAPPED = 18,
     *    SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE = 19,
     *    SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED = 20,
     *    SPELL_FAILED_CANT_STEALTH = 21,
     *    SPELL_FAILED_CASTER_AURASTATE = 22,
     *    SPELL_FAILED_CASTER_DEAD = 23,
     *    SPELL_FAILED_CHARMED = 24,
     *    SPELL_FAILED_CHEST_IN_USE = 25,
     *    SPELL_FAILED_CONFUSED = 26,
     *    SPELL_FAILED_DONT_REPORT = 27,
     *    SPELL_FAILED_EQUIPPED_ITEM = 28,
     *    SPELL_FAILED_EQUIPPED_ITEM_CLASS = 29,
     *    SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND = 30,
     *    SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND = 31,
     *    SPELL_FAILED_ERROR = 32,
     *    SPELL_FAILED_FIZZLE = 33,
     *    SPELL_FAILED_FLEEING = 34,
     *    SPELL_FAILED_FOOD_LOWLEVEL = 35,
     *    SPELL_FAILED_HIGHLEVEL = 36,
     *    SPELL_FAILED_HUNGER_SATIATED = 37,
     *    SPELL_FAILED_IMMUNE = 38,
     *    SPELL_FAILED_INCORRECT_AREA = 39,
     *    SPELL_FAILED_INTERRUPTED = 40,
     *    SPELL_FAILED_INTERRUPTED_COMBAT = 41,
     *    SPELL_FAILED_ITEM_ALREADY_ENCHANTED = 42,
     *    SPELL_FAILED_ITEM_GONE = 43,
     *    SPELL_FAILED_ITEM_NOT_FOUND = 44,
     *    SPELL_FAILED_ITEM_NOT_READY = 45,
     *    SPELL_FAILED_LEVEL_REQUIREMENT = 46,
     *    SPELL_FAILED_LINE_OF_SIGHT = 47,
     *    SPELL_FAILED_LOWLEVEL = 48,
     *    SPELL_FAILED_LOW_CASTLEVEL = 49,
     *    SPELL_FAILED_MAINHAND_EMPTY = 50,
     *    SPELL_FAILED_MOVING = 51,
     *    SPELL_FAILED_NEED_AMMO = 52,
     *    SPELL_FAILED_NEED_AMMO_POUCH = 53,
     *    SPELL_FAILED_NEED_EXOTIC_AMMO = 54,
     *    SPELL_FAILED_NEED_MORE_ITEMS = 55,
     *    SPELL_FAILED_NOPATH = 56,
     *    SPELL_FAILED_NOT_BEHIND = 57,
     *    SPELL_FAILED_NOT_FISHABLE = 58,
     *    SPELL_FAILED_NOT_FLYING = 59,
     *    SPELL_FAILED_NOT_HERE = 60,
     *    SPELL_FAILED_NOT_INFRONT = 61,
     *    SPELL_FAILED_NOT_IN_CONTROL = 62,
     *    SPELL_FAILED_NOT_KNOWN = 63,
     *    SPELL_FAILED_NOT_MOUNTED = 64,
     *    SPELL_FAILED_NOT_ON_TAXI = 65,
     *    SPELL_FAILED_NOT_ON_TRANSPORT = 66,
     *    SPELL_FAILED_NOT_READY = 67,
     *    SPELL_FAILED_NOT_SHAPESHIFT = 68,
     *    SPELL_FAILED_NOT_STANDING = 69,
     *    SPELL_FAILED_NOT_TRADEABLE = 70,
     *    SPELL_FAILED_NOT_TRADING = 71,
     *    SPELL_FAILED_NOT_UNSHEATHED = 72,
     *    SPELL_FAILED_NOT_WHILE_GHOST = 73,
     *    SPELL_FAILED_NOT_WHILE_LOOTING = 74,
     *    SPELL_FAILED_NO_AMMO = 75,
     *    SPELL_FAILED_NO_CHARGES_REMAIN = 76,
     *    SPELL_FAILED_NO_CHAMPION = 77,
     *    SPELL_FAILED_NO_COMBO_POINTS = 78,
     *    SPELL_FAILED_NO_DUELING = 79,
     *    SPELL_FAILED_NO_ENDURANCE = 80,
     *    SPELL_FAILED_NO_FISH = 81,
     *    SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED = 82,
     *    SPELL_FAILED_NO_MOUNTS_ALLOWED = 83,
     *    SPELL_FAILED_NO_PET = 84,
     *    SPELL_FAILED_NO_POWER = 85,
     *    SPELL_FAILED_NOTHING_TO_DISPEL = 86,
     *    SPELL_FAILED_NOTHING_TO_STEAL = 87,
     *    SPELL_FAILED_ONLY_ABOVEWATER = 88,
     *    SPELL_FAILED_ONLY_DAYTIME = 89,
     *    SPELL_FAILED_ONLY_INDOORS = 90,
     *    SPELL_FAILED_ONLY_MOUNTED = 91,
     *    SPELL_FAILED_ONLY_NIGHTTIME = 92,
     *    SPELL_FAILED_ONLY_OUTDOORS = 93,
     *    SPELL_FAILED_ONLY_SHAPESHIFT = 94,
     *    SPELL_FAILED_ONLY_STEALTHED = 95,
     *    SPELL_FAILED_ONLY_UNDERWATER = 96,
     *    SPELL_FAILED_OUT_OF_RANGE = 97,
     *    SPELL_FAILED_PACIFIED = 98,
     *    SPELL_FAILED_POSSESSED = 99,
     *    SPELL_FAILED_REAGENTS = 100,
     *    SPELL_FAILED_REQUIRES_AREA = 101,
     *    SPELL_FAILED_REQUIRES_SPELL_FOCUS = 102,
     *    SPELL_FAILED_ROOTED = 103,
     *    SPELL_FAILED_SILENCED = 104,
     *    SPELL_FAILED_SPELL_IN_PROGRESS = 105,
     *    SPELL_FAILED_SPELL_LEARNED = 106,
     *    SPELL_FAILED_SPELL_UNAVAILABLE = 107,
     *    SPELL_FAILED_STUNNED = 108,
     *    SPELL_FAILED_TARGETS_DEAD = 109,
     *    SPELL_FAILED_TARGET_AFFECTING_COMBAT = 110,
     *    SPELL_FAILED_TARGET_AURASTATE = 111,
     *    SPELL_FAILED_TARGET_DUELING = 112,
     *    SPELL_FAILED_TARGET_ENEMY = 113,
     *    SPELL_FAILED_TARGET_ENRAGED = 114,
     *    SPELL_FAILED_TARGET_FRIENDLY = 115,
     *    SPELL_FAILED_TARGET_IN_COMBAT = 116,
     *    SPELL_FAILED_TARGET_IS_PLAYER = 117,
     *    SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED = 118,
     *    SPELL_FAILED_TARGET_NOT_DEAD = 119,
     *    SPELL_FAILED_TARGET_NOT_IN_PARTY = 120,
     *    SPELL_FAILED_TARGET_NOT_LOOTED = 121,
     *    SPELL_FAILED_TARGET_NOT_PLAYER = 122,
     *    SPELL_FAILED_TARGET_NO_POCKETS = 123,
     *    SPELL_FAILED_TARGET_NO_WEAPONS = 124,
     *    SPELL_FAILED_TARGET_NO_RANGED_WEAPONS = 125,
     *    SPELL_FAILED_TARGET_UNSKINNABLE = 126,
     *    SPELL_FAILED_THIRST_SATIATED = 127,
     *    SPELL_FAILED_TOO_CLOSE = 128,
     *    SPELL_FAILED_TOO_MANY_OF_ITEM = 129,
     *    SPELL_FAILED_TOTEM_CATEGORY = 130,
     *    SPELL_FAILED_TOTEMS = 131,
     *    SPELL_FAILED_TRY_AGAIN = 132,
     *    SPELL_FAILED_UNIT_NOT_BEHIND = 133,
     *    SPELL_FAILED_UNIT_NOT_INFRONT = 134,
     *    SPELL_FAILED_WRONG_PET_FOOD = 135,
     *    SPELL_FAILED_NOT_WHILE_FATIGUED = 136,
     *    SPELL_FAILED_TARGET_NOT_IN_INSTANCE = 137,
     *    SPELL_FAILED_NOT_WHILE_TRADING = 138,
     *    SPELL_FAILED_TARGET_NOT_IN_RAID = 139,
     *    SPELL_FAILED_TARGET_FREEFORALL = 140,
     *    SPELL_FAILED_NO_EDIBLE_CORPSES = 141,
     *    SPELL_FAILED_ONLY_BATTLEGROUNDS = 142,
     *    SPELL_FAILED_TARGET_NOT_GHOST = 143,
     *    SPELL_FAILED_TRANSFORM_UNUSABLE = 144,
     *    SPELL_FAILED_WRONG_WEATHER = 145,
     *    SPELL_FAILED_DAMAGE_IMMUNE = 146,
     *    SPELL_FAILED_PREVENTED_BY_MECHANIC = 147,
     *    SPELL_FAILED_PLAY_TIME = 148,
     *    SPELL_FAILED_REPUTATION = 149,
     *    SPELL_FAILED_MIN_SKILL = 150,
     *    SPELL_FAILED_NOT_IN_ARENA = 151,
     *    SPELL_FAILED_NOT_ON_SHAPESHIFT = 152,
     *    SPELL_FAILED_NOT_ON_STEALTHED = 153,
     *    SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE = 154,
     *    SPELL_FAILED_NOT_ON_MOUNTED = 155,
     *    SPELL_FAILED_TOO_SHALLOW = 156,
     *    SPELL_FAILED_TARGET_NOT_IN_SANCTUARY = 157,
     *    SPELL_FAILED_TARGET_IS_TRIVIAL = 158,
     *    SPELL_FAILED_BM_OR_INVISGOD = 159,
     *    SPELL_FAILED_EXPERT_RIDING_REQUIREMENT = 160,
     *    SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT = 161,
     *    SPELL_FAILED_NOT_IDLE = 162,
     *    SPELL_FAILED_NOT_INACTIVE = 163,
     *    SPELL_FAILED_PARTIAL_PLAYTIME = 164,
     *    SPELL_FAILED_NO_PLAYTIME = 165,
     *    SPELL_FAILED_NOT_IN_BATTLEGROUND = 166,
     *    SPELL_FAILED_NOT_IN_RAID_INSTANCE = 167,
     *    SPELL_FAILED_ONLY_IN_ARENA = 168,
     *    SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE = 169,
     *    SPELL_FAILED_ON_USE_ENCHANT = 170,
     *    SPELL_FAILED_NOT_ON_GROUND = 171,
     *    SPELL_FAILED_CUSTOM_ERROR = 172,
     *    SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW = 173,
     *    SPELL_FAILED_TOO_MANY_SOCKETS = 174,
     *    SPELL_FAILED_INVALID_GLYPH = 175,
     *    SPELL_FAILED_UNIQUE_GLYPH = 176,
     *    SPELL_FAILED_GLYPH_SOCKET_LOCKED = 177,
     *    SPELL_FAILED_NO_VALID_TARGETS = 178,
     *    SPELL_FAILED_ITEM_AT_MAX_CHARGES = 179,
     *    SPELL_FAILED_NOT_IN_BARBERSHOP = 180,
     *    SPELL_FAILED_FISHING_TOO_LOW = 181,
     *    SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW = 182,
     *    SPELL_FAILED_SUMMON_PENDING = 183,
     *    SPELL_FAILED_MAX_SOCKETS = 184,
     *    SPELL_FAILED_PET_CAN_RENAME = 185,
     *    SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED = 186,
     *    SPELL_FAILED_UNKNOWN = 187, // actually doesn't exist in client
     *
     *    SPELL_CAST_OK = 255 // custom value, must not be sent to client
     * };
     *
     * @param [uint8] castResult : Specified result
     */
    int SendCastResult(lua_State* L, Spell* spell)
    {
        uint8 result = Forge::CHECKVAL<uint8>(L, 2);
        spell->SendCastResult((SpellCastResult)result);
        return 0;
    }

    ForgeRegister<Spell> SpellMethods[] =
    {
        // Getters
        { "GetCaster", &LuaSpell::GetCaster },
        { "GetCastTime", &LuaSpell::GetCastTime },
        { "GetEntry", &LuaSpell::GetEntry },
        { "GetDuration", &LuaSpell::GetDuration },
        { "GetPowerCost", &LuaSpell::GetPowerCost },
        { "GetReagentCost", &LuaSpell::GetReagentCost },
        { "GetTargetDest", &LuaSpell::GetTargetDest },
        { "GetTarget", &LuaSpell::GetTarget },
        { "GetSpellInfo", &LuaSpell::GetSpellInfo },

        // Setters
        { "SetAutoRepeat", &LuaSpell::SetAutoRepeat },

        // Boolean
        { "IsAutoRepeat", &LuaSpell::IsAutoRepeat },

        // Other
        { "Cancel", &LuaSpell::Cancel },
        { "Cast", &LuaSpell::Cast },
        { "Finish", &LuaSpell::Finish },
        { "SendCastResult", &LuaSpell::SendCastResult },

        { NULL, NULL }
    };
};
#endif
