#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "nbt/CompoundTag.h"

class Packet;
class PacketListener;
class DataInputStream;
class DataOutputStream;
class CompoundTag;

#define PACKET_ENABLE_STAT_TRACKING 0

class Packet;
class ItemInstance;

typedef std::shared_ptr<Packet> (*packetCreateFn)();

class Packet {
public:
    class PacketStatistics {
    private:
        int count;
        int totalSize;

        static const int TOTAL_TICKS = 100;

        // 4J Added
        int64_t countSamples[TOTAL_TICKS];
        int64_t sizeSamples[TOTAL_TICKS];
        int64_t timeSamples[TOTAL_TICKS];
        int samplesPos;

    public:
        const int id;

    public:
        PacketStatistics(int id);
        void addPacket(int bytes);
        int getCount();
        int getTotalSize();
        double getAverageSize();
        int64_t getRunningTotal();
        int64_t getRunningCount();
        void IncrementPos();
    };

    // 4J JEV, replaces the static blocks.
    static void staticCtor();

public:
    static std::unordered_map<int, packetCreateFn>
        idToCreateMap;  // IntHashMap in 1.8.2 ... needed? // Made public
                        // in 1.0.1

    static std::unordered_set<int> clientReceivedPackets;
    static std::unordered_set<int> serverReceivedPackets;
    static std::unordered_set<int> sendToAnyClientPackets;

    // 4J Stu - Added the sendToAnyClient param so we can limit some packets to
    // be only sent to one player on a system 4J Stu - Added renderStats param
    // for use in debugging
    static void map(int id, bool receiveOnClient, bool receiveOnServer,
                    bool sendToAnyClient, bool renderStats,
                    const std::type_info& clazz, packetCreateFn);

public:
    const int64_t createTime;

    Packet();

    static std::shared_ptr<Packet> getPacket(int id);

    // 4J Added
    static bool canSendToAnyClient(std::shared_ptr<Packet> packet);

    static void writeBytes(DataOutputStream* dataoutputstream,
                           const std::vector<uint8_t>& bytes);
    static std::vector<uint8_t> readBytes(DataInputStream* datainputstream);

    virtual int getId() = 0;

    bool shouldDelay;

private:
    // 4J Added to track stats for packets that are going out via QNet
    static std::unordered_map<int, PacketStatistics*>
        outgoingStatistics;  // IntHashMap in 1.8.2 ... needed?
    static std::vector<PacketStatistics*> renderableStats;
    static int renderPos;

public:
    static void recordOutgoingPacket(std::shared_ptr<Packet> packet,
                                     int playerIndex);
    static void updatePacketStatsPIX();

private:
    static std::unordered_map<int, PacketStatistics*> statistics;
    // static int nextPrint;

public:
    static std::shared_ptr<Packet> readPacket(DataInputStream* dis,
                                              bool isServer);
    static void writePacket(std::shared_ptr<Packet> packet,
                            DataOutputStream* dos);
    static void writeUtf(const std::string& value, DataOutputStream* dos);
    static std::string readUtf(DataInputStream* dis, int maxLength);
    virtual void read(
        DataInputStream* dis) = 0;  // throws IOException = 0; TODO 4J JEV,
                                    // should this declare a throws?
    virtual void write(
        DataOutputStream* dos) = 0;  // throws IOException = 0; TODO 4J JEV,
                                     // should this declare a throws?
    virtual void handle(PacketListener* listener) = 0;
    virtual int getEstimatedSize() = 0;
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);
    virtual bool isAync();

    // 4J Stu - Brought these functions forward for enchanting/game rules
    static std::shared_ptr<ItemInstance> readItem(DataInputStream* dis);
    static void writeItem(std::shared_ptr<ItemInstance> item,
                          DataOutputStream* dos);
    static CompoundTag* readNbt(DataInputStream* dis);

protected:
    static void writeNbt(CompoundTag* tag, DataOutputStream* dos);
};