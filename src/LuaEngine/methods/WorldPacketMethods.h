/*
* Copyright (C) 2010 - 2016 Forge Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef WORLDPACKETMETHODS_H
#define WORLDPACKETMETHODS_H

/***
 * A packet used to pass messages between the server and a client.
 *
 * Each packet has an opcode that determines the type of message being sent,
 *   e.g. if a CMSG_LOGOUT_REQUEST packet is sent to the server,
 *   the client has sent a message that its [Player] wants to logout.
 *
 * The packet can contain further data, the format of which depends on the opcode.
 *
 * Inherits all methods from: none
 */
namespace LuaPacket
{
    /**
     * Returns the opcode of the [WorldPacket].
     *
     * @return uint16 opcode
     */
    int GetOpcode(lua_State* L, WorldPacket* packet)
    {
        Forge::Push(L, packet->GetOpcode());
        return 1;
    }

    /**
     * Returns the size of the [WorldPacket].
     *
     * @return uint32 size
     */
    int GetSize(lua_State* L, WorldPacket* packet)
    {
        Forge::Push(L, packet->size());
        return 1;
    }

    /**
     * Sets the opcode of the [WorldPacket] to the specified opcode.
     *
     * @param [Opcodes] opcode : see Opcodes.h for all known opcodes
     */
    int SetOpcode(lua_State* L, WorldPacket* packet)
    {
        uint32 opcode = Forge::CHECKVAL<uint32>(L, 2);
        if (opcode >= NUM_MSG_TYPES)
            return luaL_argerror(L, 2, "valid opcode expected");
        packet->SetOpcode((OpcodesList)opcode);
        return 0;
    }

    /**
     * Reads and returns a signed 8-bit integer value from the [WorldPacket].
     *
     * @return int8 value
     */
    int ReadByte(lua_State* L, WorldPacket* packet)
    {
        int8 _byte;
        (*packet) >> _byte;
        Forge::Push(L, _byte);
        return 1;
    }

    /**
     * Reads and returns an unsigned 8-bit integer value from the [WorldPacket].
     *
     * @return uint8 value
     */
    int ReadUByte(lua_State* L, WorldPacket* packet)
    {
        uint8 _ubyte;
        (*packet) >> _ubyte;
        Forge::Push(L, _ubyte);
        return 1;
    }

    /**
     * Reads and returns a signed 16-bit integer value from the [WorldPacket].
     *
     * @return int16 value
     */
    int ReadShort(lua_State* L, WorldPacket* packet)
    {
        int16 _short;
        (*packet) >> _short;
        Forge::Push(L, _short);
        return 1;
    }

    /**
     * Reads and returns an unsigned 16-bit integer value from the [WorldPacket].
     *
     * @return uint16 value
     */
    int ReadUShort(lua_State* L, WorldPacket* packet)
    {
        uint16 _ushort;
        (*packet) >> _ushort;
        Forge::Push(L, _ushort);
        return 1;
    }

    /**
     * Reads and returns a signed 32-bit integer value from the [WorldPacket].
     *
     * @return int32 value
     */
    int ReadLong(lua_State* L, WorldPacket* packet)
    {
        int32 _long;
        (*packet) >> _long;
        Forge::Push(L, _long);
        return 1;
    }

    /**
     * Reads and returns an unsigned 32-bit integer value from the [WorldPacket].
     *
     * @return uint32 value
     */
    int ReadULong(lua_State* L, WorldPacket* packet)
    {
        uint32 _ulong;
        (*packet) >> _ulong;
        Forge::Push(L, _ulong);
        return 1;
    }

    /**
     * Reads and returns a single-precision floating-point value from the [WorldPacket].
     *
     * @return float value
     */
    int ReadFloat(lua_State* L, WorldPacket* packet)
    {
        float _val;
        (*packet) >> _val;
        Forge::Push(L, _val);
        return 1;
    }

    /**
     * Reads and returns a double-precision floating-point value from the [WorldPacket].
     *
     * @return double value
     */
    int ReadDouble(lua_State* L, WorldPacket* packet)
    {
        double _val;
        (*packet) >> _val;
        Forge::Push(L, _val);
        return 1;
    }

    /**
     * Reads and returns an unsigned 64-bit integer value from the [WorldPacket].
     *
     * @return ObjectGuid value : value returned as string
     */
    int ReadGUID(lua_State* L, WorldPacket* packet)
    {
        ObjectGuid guid;
        (*packet) >> guid;
        Forge::Push(L, guid);
        return 1;
    }

    /**
     * Reads and returns a string value from the [WorldPacket].
     *
     * @return string value
     */
    int ReadString(lua_State* L, WorldPacket* packet)
    {
        std::string _val;
        (*packet) >> _val;
        Forge::Push(L, _val);
        return 1;
    }

    /**
     * Writes an unsigned 64-bit integer value to the [WorldPacket].
     *
     * @param ObjectGuid value : the value to be written to the [WorldPacket]
     */
    int WriteGUID(lua_State* L, WorldPacket* packet)
    {
        ObjectGuid guid = Forge::CHECKVAL<ObjectGuid>(L, 2);
        (*packet) << guid;
        return 0;
    }

    /**
     * Writes a string to the [WorldPacket].
     *
     * @param string value : the string to be written to the [WorldPacket]
     */
    int WriteString(lua_State* L, WorldPacket* packet)
    {
        std::string _val = Forge::CHECKVAL<std::string>(L, 2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes a signed 8-bit integer value to the [WorldPacket].
     *
     * @param int8 value : the int8 value to be written to the [WorldPacket]
     */
    int WriteByte(lua_State* L, WorldPacket* packet)
    {
        int8 byte = Forge::CHECKVAL<int8>(L, 2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes an unsigned 8-bit integer value to the [WorldPacket].
     *
     * @param uint8 value : the uint8 value to be written to the [WorldPacket]
     */
    int WriteUByte(lua_State* L, WorldPacket* packet)
    {
        uint8 byte = Forge::CHECKVAL<uint8>(L, 2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes a signed 16-bit integer value to the [WorldPacket].
     *
     * @param int16 value : the int16 value to be written to the [WorldPacket]
     */
    int WriteShort(lua_State* L, WorldPacket* packet)
    {
        int16 _short = Forge::CHECKVAL<int16>(L, 2);
        (*packet) << _short;
        return 0;
    }

    /**
     * Writes an unsigned 16-bit integer value to the [WorldPacket].
     *
     * @param uint16 value : the uint16 value to be written to the [WorldPacket]
     */
    int WriteUShort(lua_State* L, WorldPacket* packet)
    {
        uint16 _ushort = Forge::CHECKVAL<uint16>(L, 2);
        (*packet) << _ushort;
        return 0;
    }

    /**
     * Writes a signed 32-bit integer value to the [WorldPacket].
     *
     * @param int32 value : the int32 value to be written to the [WorldPacket]
     */
    int WriteLong(lua_State* L, WorldPacket* packet)
    {
        int32 _long = Forge::CHECKVAL<int32>(L, 2);
        (*packet) << _long;
        return 0;
    }

    /**
     * Writes an unsigned 32-bit integer value to the [WorldPacket].
     *
     * @param uint32 value : the uint32 value to be written to the [WorldPacket]
     */
    int WriteULong(lua_State* L, WorldPacket* packet)
    {
        uint32 _ulong = Forge::CHECKVAL<uint32>(L, 2);
        (*packet) << _ulong;
        return 0;
    }

    /**
     * Writes a 32-bit floating-point value to the [WorldPacket].
     *
     * @param float value : the float value to be written to the [WorldPacket]
     */
    int WriteFloat(lua_State* L, WorldPacket* packet)
    {
        float _val = Forge::CHECKVAL<float>(L, 2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes a 64-bit floating-point value to the [WorldPacket].
     *
     * @param double value : the double value to be written to the [WorldPacket]
     */
    int WriteDouble(lua_State* L, WorldPacket* packet)
    {
        double _val = Forge::CHECKVAL<double>(L, 2);
        (*packet) << _val;
        return 0;
    }
};

#endif
