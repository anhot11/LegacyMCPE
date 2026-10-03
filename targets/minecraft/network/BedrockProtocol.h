#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

namespace Bedrock {

// MCPE 0.15.10.0 Protocol Version
constexpr int PROTOCOL_VERSION_0_15 = 84;
constexpr int DEFAULT_PORT = 19132;

// RakNet Protocol Magic (16 bytes)
constexpr uint8_t RAKNET_MAGIC[16] = {
    0x00, 0xff, 0xff, 0x00,
    0xfe, 0xfe, 0xfe, 0xfe,
    0xfd, 0xfd, 0xfd, 0xfd,
    0x12, 0x34, 0x56, 0x78
};

// RakNet Packet IDs
namespace RakNetPacketId {
    constexpr uint8_t CONNECTED_PING             = 0x00;
    constexpr uint8_t UNCONNECTED_PING           = 0x01;
    constexpr uint8_t UNCONNECTED_PING_OPEN      = 0x02;
    constexpr uint8_t CONNECTED_PONG             = 0x03;
    constexpr uint8_t OPEN_CONNECTION_REQUEST_1  = 0x05;
    constexpr uint8_t OPEN_CONNECTION_REPLY_1    = 0x06;
    constexpr uint8_t OPEN_CONNECTION_REQUEST_2  = 0x07;
    constexpr uint8_t OPEN_CONNECTION_REPLY_2    = 0x08;
    constexpr uint8_t CONNECTION_REQUEST         = 0x09;
    constexpr uint8_t CONNECTION_REQUEST_ACCEPTED= 0x10;
    constexpr uint8_t NEW_INCOMING_CONNECTION    = 0x13;
    constexpr uint8_t DISCONNECTION_NOTIFICATION = 0x15;
    constexpr uint8_t UNCONNECTED_PONG           = 0x1c;
    constexpr uint8_t INCOMPATIBLE_PROTOCOL      = 0x19;
}

// Bedrock 0.15.10 (Protocol 84) Game Packet IDs
namespace PacketId {
    constexpr uint8_t LOGIN                     = 0x01; // 0x82 batch
    constexpr uint8_t PLAY_STATUS               = 0x02;
    constexpr uint8_t SERVER_TO_CLIENT_HANDSHAKE= 0x03;
    constexpr uint8_t CLIENT_TO_SERVER_HANDSHAKE= 0x04;
    constexpr uint8_t DISCONNECT                = 0x05;
    constexpr uint8_t BATCH                     = 0x06;
    constexpr uint8_t RESOURCE_PACKS_INFO       = 0x07;
    constexpr uint8_t RESOURCE_PACK_STACK       = 0x08;
    constexpr uint8_t RESOURCE_PACK_CLIENT_RESPONSE = 0x09;
    constexpr uint8_t TEXT                      = 0x0a;
    constexpr uint8_t SET_TIME                  = 0x0b;
    constexpr uint8_t START_GAME                = 0x0c;
    constexpr uint8_t ADD_PLAYER                = 0x0d;
    constexpr uint8_t ADD_ENTITY                = 0x0e;
    constexpr uint8_t REMOVE_ENTITY             = 0x0f;
    constexpr uint8_t ADD_ITEM_ENTITY           = 0x10;
    constexpr uint8_t TAKE_ITEM_ENTITY          = 0x11;
    constexpr uint8_t MOVE_ENTITY               = 0x12;
    constexpr uint8_t MOVE_PLAYER               = 0x13;
    constexpr uint8_t REMOVE_BLOCK              = 0x14;
    constexpr uint8_t UPDATE_BLOCK              = 0x15;
    constexpr uint8_t ADD_PAINTING              = 0x16;
    constexpr uint8_t EXPLODE                   = 0x17;
    constexpr uint8_t LEVEL_EVENT               = 0x18;
    constexpr uint8_t BLOCK_EVENT               = 0x19;
    constexpr uint8_t ENTITY_EVENT              = 0x1a;
    constexpr uint8_t MOB_EFFECT                = 0x1b;
    constexpr uint8_t UPDATE_ATTRIBUTES         = 0x1c;
    constexpr uint8_t MOB_EQUIPMENT             = 0x1d;
    constexpr uint8_t MOB_ARMOR_EQUIPMENT       = 0x1e;
    constexpr uint8_t INTERACT                  = 0x1f;
    constexpr uint8_t USE_ITEM                  = 0x20;
    constexpr uint8_t PLAYER_ACTION             = 0x21;
    constexpr uint8_t HURT_ARMOR                = 0x22;
    constexpr uint8_t SET_ENTITY_DATA           = 0x23;
    constexpr uint8_t SET_ENTITY_MOTION         = 0x24;
    constexpr uint8_t SET_ENTITY_LINK           = 0x25;
    constexpr uint8_t SET_HEALTH                = 0x26;
    constexpr uint8_t SET_SPAWN_POSITION        = 0x27;
    constexpr uint8_t ANIMATE                   = 0x28;
    constexpr uint8_t RESPAWN                   = 0x29;
    constexpr uint8_t DROP_ITEM                 = 0x2a;
    constexpr uint8_t CONTAINER_OPEN            = 0x2b;
    constexpr uint8_t CONTAINER_CLOSE           = 0x2c;
    constexpr uint8_t CONTAINER_SET_SLOT        = 0x2d;
    constexpr uint8_t CONTAINER_SET_DATA        = 0x2e;
    constexpr uint8_t CONTAINER_SET_CONTENT     = 0x2f;
    constexpr uint8_t CRAFTING_DATA             = 0x30;
    constexpr uint8_t CRAFTING_EVENT            = 0x31;
    constexpr uint8_t ADVENTURE_SETTINGS        = 0x32;
    constexpr uint8_t BLOCK_ENTITY_DATA         = 0x33;
    constexpr uint8_t PLAYER_INPUT              = 0x34;
    constexpr uint8_t FULL_CHUNK_DATA           = 0x35;
    constexpr uint8_t SET_DIFFICULTY            = 0x36;
    constexpr uint8_t CHANGE_DIMENSION          = 0x37;
    constexpr uint8_t SET_PLAYER_GAMETYPE       = 0x38;
    constexpr uint8_t PLAYER_LIST               = 0x39;
    constexpr uint8_t TELEMETRY_EVENT           = 0x3a;
    constexpr uint8_t SPAWN_EXPERIENCE_ORB      = 0x3b;
    constexpr uint8_t CLIENTBOUND_MAP_ITEM_DATA = 0x3c;
    constexpr uint8_t MAP_INFO_REQUEST          = 0x3d;
    constexpr uint8_t REQUEST_CHUNK_RADIUS      = 0x3e;
    constexpr uint8_t CHUNK_RADIUS_UPDATED      = 0x3f;
    constexpr uint8_t ITEM_FRAME_DROP_ITEM      = 0x40;
    constexpr uint8_t REPLACE_ITEM_IN_SLOT      = 0x41;
    constexpr uint8_t GAME_RULES_CHANGED        = 0x42;
    constexpr uint8_t CAMERA                    = 0x43;
    constexpr uint8_t ADD_ITEM                  = 0x44;
    constexpr uint8_t BOSS_EVENT                = 0x45;
    constexpr uint8_t SHOW_CREDITS              = 0x46;
    constexpr uint8_t AVAILABLE_COMMANDS        = 0x47;
    constexpr uint8_t COMMAND_REQUEST           = 0x48;
    constexpr uint8_t COMMAND_BLOCK_UPDATE      = 0x49;
    constexpr uint8_t COMMAND_OUTPUT            = 0x4a;
    constexpr uint8_t UPDATE_TRADE              = 0x4b;
    constexpr uint8_t UPDATE_EQUIP              = 0x4c;
    constexpr uint8_t RESOURCE_PACK_DATA_INFO   = 0x4d;
    constexpr uint8_t RESOURCE_PACK_CHUNK_DATA  = 0x4e;
    constexpr uint8_t RESOURCE_PACK_CHUNK_REQUEST = 0x4f;
    constexpr uint8_t TRANSFER                  = 0x50;
    constexpr uint8_t PLAY_SOUND                = 0x51;
    constexpr uint8_t STOP_SOUND                = 0x52;
    constexpr uint8_t SET_TITLE                 = 0x53;
}

// PlayStatus values
namespace PlayStatus {
    constexpr int LOGIN_SUCCESS                 = 0;
    constexpr int FAILED_CLIENT_OUTDATED        = 1;
    constexpr int FAILED_SERVER_OUTDATED        = 2;
    constexpr int PLAYER_SPAWN                  = 3;
    constexpr int FAILED_INVALID_TENANT         = 4;
    constexpr int FAILED_VANILLA_EDU_RESTRICTION= 5;
    constexpr int FAILED_EDU_VANILLA_RESTRICTION= 6;
    constexpr int FAILED_SERVER_FULL            = 7;
}

// Helpers for buffer serialization
class BufferWriter {
public:
    std::vector<uint8_t> buffer;

    void writeByte(uint8_t b) { buffer.push_back(b); }
    void writeBytes(const uint8_t* data, size_t len) {
        buffer.insert(buffer.end(), data, data + len);
    }
    void writeMagic() { writeBytes(RAKNET_MAGIC, 16); }

    void writeShortBE(uint16_t v) {
        writeByte((v >> 8) & 0xff);
        writeByte(v & 0xff);
    }

    void writeIntBE(uint32_t v) {
        writeByte((v >> 24) & 0xff);
        writeByte((v >> 16) & 0xff);
        writeByte((v >> 8) & 0xff);
        writeByte(v & 0xff);
    }

    void writeIntLE(uint32_t v) {
        writeByte(v & 0xff);
        writeByte((v >> 8) & 0xff);
        writeByte((v >> 16) & 0xff);
        writeByte((v >> 24) & 0xff);
    }

    void writeLongBE(uint64_t v) {
        for (int i = 7; i >= 0; --i) {
            writeByte((v >> (i * 8)) & 0xff);
        }
    }

    void writeString(const std::string& str) {
        writeShortBE(static_cast<uint16_t>(str.size()));
        writeBytes(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    void writeVarInt(int32_t value) {
        uint32_t uval = static_cast<uint32_t>((value << 1) ^ (value >> 31));
        while (uval >= 0x80) {
            writeByte(static_cast<uint8_t>(uval | 0x80));
            uval >>= 7;
        }
        writeByte(static_cast<uint8_t>(uval));
    }
};

class BufferReader {
private:
    const uint8_t* data;
    size_t size;
    size_t offset;

public:
    BufferReader(const uint8_t* ptr, size_t len) : data(ptr), size(len), offset(0) {}

    bool hasRemaining(size_t n = 1) const { return offset + n <= size; }
    size_t getRemaining() const { return size > offset ? size - offset : 0; }

    uint8_t readByte() {
        if (!hasRemaining(1)) return 0;
        return data[offset++];
    }

    uint16_t readShortBE() {
        if (!hasRemaining(2)) return 0;
        uint16_t v = (static_cast<uint16_t>(data[offset]) << 8) | data[offset + 1];
        offset += 2;
        return v;
    }

    uint32_t readIntBE() {
        if (!hasRemaining(4)) return 0;
        uint32_t v = (static_cast<uint32_t>(data[offset]) << 24) |
                     (static_cast<uint32_t>(data[offset + 1]) << 16) |
                     (static_cast<uint32_t>(data[offset + 2]) << 8) |
                     data[offset + 3];
        offset += 4;
        return v;
    }

    uint32_t readIntLE() {
        if (!hasRemaining(4)) return 0;
        uint32_t v = data[offset] |
                     (static_cast<uint32_t>(data[offset + 1]) << 8) |
                     (static_cast<uint32_t>(data[offset + 2]) << 16) |
                     (static_cast<uint32_t>(data[offset + 3]) << 24);
        offset += 4;
        return v;
    }

    std::string readString() {
        uint16_t len = readShortBE();
        if (!hasRemaining(len)) return "";
        std::string s(reinterpret_cast<const char*>(data + offset), len);
        offset += len;
        return s;
    }

    int32_t readVarInt() {
        uint32_t result = 0;
        int shift = 0;
        while (hasRemaining(1)) {
            uint8_t b = readByte();
            result |= (b & 0x7f) << shift;
            if (!(b & 0x80)) break;
            shift += 7;
            if (shift >= 35) break;
        }
        return static_cast<int32_t>((result >> 1) ^ -(static_cast<int32_t>(result & 1)));
    }
};

} // namespace Bedrock
