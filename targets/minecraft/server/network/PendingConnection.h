#pragma once
#include <memory>
#include <string>

#include "minecraft/network/packet/DisconnectPacket.h"
#include "minecraft/network/packet/PacketListener.h"

class MinecraftServer;
class Socket;
class LoginPacket;
class Connection;
class Random;
class Packet;

class PendingConnection : public PacketListener {
private:
    static const int FAKE_LAG = 0;
    static const int MAX_TICKS_BEFORE_LOGIN =
        20 * 30 *
        10;  // 10 minutes instead of 20 sec for Linux theres just no login yet

    //    public static Logger logger = Logger.getLogger("Minecraft");
    static Random* random;

public:
    Connection* connection;

public:
    bool done;

private:
    MinecraftServer* server;
    int _tick;
    std::string name;
    std::shared_ptr<LoginPacket> acceptedLogin;
    std::string loginKey;

public:
    PendingConnection(MinecraftServer* server, Socket* socket,
                      const std::string& id);
    ~PendingConnection();
    void tick();
    void disconnect(DisconnectPacket::eDisconnectReason reason);
    virtual void handlePreLogin(std::shared_ptr<PreLoginPacket> packet);
    virtual void handleLogin(std::shared_ptr<LoginPacket> packet);
    virtual void handleAcceptedLogin(std::shared_ptr<LoginPacket> packet);
    virtual void onDisconnect(DisconnectPacket::eDisconnectReason reason,
                              void* reasonObjects);
    virtual void handleGetInfo(std::shared_ptr<GetInfoPacket> packet);
    virtual void handleKeepAlive(std::shared_ptr<KeepAlivePacket> packet);
    virtual void onUnhandledPacket(std::shared_ptr<Packet> packet);
    void send(std::shared_ptr<Packet> packet);
    std::string getName();
    virtual bool isServerPacketListener();
    virtual bool isDisconnected();

private:
    void sendPreLoginResponse();
};