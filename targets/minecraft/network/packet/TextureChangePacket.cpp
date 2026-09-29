#include "TextureChangePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/entity/Entity.h"

TextureChangePacket::TextureChangePacket() {
    id = -1;
    action = e_TextureChange_Skin;
    path = "";
}

TextureChangePacket::TextureChangePacket(std::shared_ptr<Entity> e,
                                         ETextureChangeType action,
                                         const std::string& path) {
    id = e->entityId;
    this->action = action;
    this->path = path;
}

void TextureChangePacket::read(DataInputStream* dis)  // throws IOException
{
    id = dis->readInt();
    action = (ETextureChangeType)dis->readByte();
    path = dis->readUTF();
}

void TextureChangePacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt(id);
    dos->writeByte(action);
    dos->writeUTF(path);
}

void TextureChangePacket::handle(PacketListener* listener) {
    listener->handleTextureChange(shared_from_this());
}

int TextureChangePacket::getEstimatedSize() { return 5 + (int)path.size(); }
