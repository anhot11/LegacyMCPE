#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ContainerOpenPacket
    : public Packet,
      public std::enable_shared_from_this<ContainerOpenPacket> {
public:
    static inline constexpr int CONTAINER = 0;
    static inline constexpr int WORKBENCH = 1;
    static inline constexpr int FURNACE = 2;
    static inline constexpr int TRAP = 3;
    static inline constexpr int ENCHANTMENT = 4;
    static inline constexpr int BREWING_STAND = 5;
    static inline constexpr int TRADER_NPC = 6;
    static inline constexpr int BEACON = 7;
    static inline constexpr int REPAIR_TABLE = 8;
    static inline constexpr int HOPPER = 9;
    static inline constexpr int DROPPER = 10;
    static inline constexpr int HORSE = 11;
    static inline constexpr int FIREWORKS = 12;        // 4J Added
    static inline constexpr int BONUS_CHEST = 13;      // 4J Added
    static inline constexpr int LARGE_CHEST = 14;      // 4J Added
    static inline constexpr int ENDER_CHEST = 15;      // 4J Added
    static inline constexpr int MINECART_CHEST = 16;   // 4J Added
    static inline constexpr int MINECART_HOPPER = 17;  // 4J Added

    int containerId;
    int type;
    int size;
    bool customName;
    std::string title;
    int entityId;

private:
    void _init(int containerId, int type, const std::string& title, int size,
               bool customName, int entityId);

public:
    ContainerOpenPacket();
    ContainerOpenPacket(int containerId, int type, const std::string& title,
                        int size, bool customName);
    ContainerOpenPacket(int containerId, int type, const std::string& title,
                        int size, bool customName, int entityId);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerOpenPacket>();
    }
    virtual int getId() { return 100; }
};
