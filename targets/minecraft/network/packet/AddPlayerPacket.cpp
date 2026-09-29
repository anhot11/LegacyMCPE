#include "AddPlayerPacket.h"

#include <vector>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/entity/SyncedEntityData.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/ItemInstance.h"

AddPlayerPacket::AddPlayerPacket() {
    id = -1;
    name = "";
    x = 0;
    y = 0;
    z = 0;
    yRot = 0;
    xRot = 0;
    carriedItem = 0;
    xuid = INVALID_XUID;
    m_playerIndex = 0;
    m_skinId = 0;
    m_capeId = 0;
    m_uiGamePrivileges = 0;
    entityData = nullptr;
    unpack = nullptr;
}

AddPlayerPacket::~AddPlayerPacket() {
    if (unpack != nullptr) delete unpack;
}

AddPlayerPacket::AddPlayerPacket(std::shared_ptr<Player> player, PlayerUID xuid,
                                 PlayerUID OnlineXuid, int xp, int yp, int zp,
                                 int yRotp, int xRotp, int yHeadRotp) {
    id = player->entityId;
    name = player->getName();

    // 4J Stu - Send "previously sent" value of position as well so that we stay
    // in sync
    x = xp;  // Mth::floor(player->x * 32);
    y = yp;  // Mth::floor(player->y * 32);
    z = zp;  // Mth::floor(player->z * 32);
    // 4J - changed - send current "previously sent" value of rotations to put
    // this in sync with other clients
    yRot = yRotp;
    xRot = xRotp;
    yHeadRot = yHeadRotp;  // 4J Added
    //    yRot = (byte) (player->yRot * 256 / 360);
    //    xRot = (byte) (player->xRot * 256 / 360);

    // printf("%d: New add player (%f,%f,%f) : (%d,%d,%d) : xRot %d, yRot
    // %d\n",id,player->x,player->y,player->z,x,y,z,xRot,yRot);

    std::shared_ptr<ItemInstance> itemInstance =
        player->inventory->getSelected();
    carriedItem = itemInstance == nullptr ? 0 : itemInstance->id;

    this->xuid = xuid;
    this->OnlineXuid = OnlineXuid;
    m_playerIndex = (uint8_t)player->getPlayerIndex();
    m_skinId = player->getCustomSkin();
    m_capeId = player->getCustomCape();
    m_uiGamePrivileges = player->getAllPlayerGamePrivileges();

    entityData = player->getEntityData();
    unpack = nullptr;
}

void AddPlayerPacket::read(DataInputStream* dis)  // throws IOException
{
    id = dis->readInt();
    name = readUtf(dis, Player::MAX_NAME_LENGTH);
    x = dis->readInt();
    y = dis->readInt();
    z = dis->readInt();
    yRot = dis->readByte();
    xRot = dis->readByte();
    yHeadRot = dis->readByte();  // 4J Added
    carriedItem = dis->readShort();
    xuid = dis->readPlayerUID();
    OnlineXuid = dis->readPlayerUID();
    m_playerIndex = dis->readByte();
    int32_t skinId = dis->readInt();
    m_skinId = *(uint32_t*)&skinId;
    int32_t capeId = dis->readInt();
    m_capeId = *(uint32_t*)&capeId;
    int32_t privileges = dis->readInt();
    m_uiGamePrivileges = *(unsigned int*)&privileges;
    unpack = SynchedEntityData::unpack(dis);
}

void AddPlayerPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt(id);
    writeUtf(name, dos);
    dos->writeInt(x);
    dos->writeInt(y);
    dos->writeInt(z);
    dos->writeByte(static_cast<std::uint8_t>(yRot));
    dos->writeByte(static_cast<std::uint8_t>(xRot));
    dos->writeByte(static_cast<std::uint8_t>(yHeadRot));  // 4J Added
    dos->writeShort(carriedItem);
    dos->writePlayerUID(xuid);
    dos->writePlayerUID(OnlineXuid);
    dos->writeByte(static_cast<std::uint8_t>(m_playerIndex));
    dos->writeInt(m_skinId);
    dos->writeInt(m_capeId);
    dos->writeInt(m_uiGamePrivileges);
    entityData->packAll(dos);
}

void AddPlayerPacket::handle(PacketListener* listener) {
    listener->handleAddPlayer(shared_from_this());
}

int AddPlayerPacket::getEstimatedSize() {
    int iSize = sizeof(int) + Player::MAX_NAME_LENGTH + sizeof(int) +
                sizeof(int) + sizeof(int) + sizeof(uint8_t) + sizeof(uint8_t) +
                sizeof(short) + sizeof(PlayerUID) + sizeof(PlayerUID) +
                sizeof(int) + sizeof(uint8_t) + sizeof(unsigned int) +
                sizeof(uint8_t);

    if (entityData != nullptr) {
        iSize += entityData->getSizeInBytes();
    } else if (unpack != nullptr) {
        // 4J Stu - This is an incoming value which we aren't currently
        // analysing
        // iSize += unpack->get
    }

    return iSize;
}

std::vector<std::shared_ptr<SynchedEntityData::DataItem> >*
AddPlayerPacket::getUnpackedData() {
    if (unpack == nullptr) {
        unpack = entityData->getAll();
    }
    return unpack;
}
