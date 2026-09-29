#include "TexturePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

TexturePacket::TexturePacket() {
    this->textureName = "";
    this->dataBytes = 0;
    this->pbData = nullptr;
}

TexturePacket::~TexturePacket() {
    // can't free this - it's used elsewhere
    // 	if(this->pbData!=nullptr)
    // 	{
    // 		delete [] this->pbData;
    // 	}
}

TexturePacket::TexturePacket(const std::string& textureName,
                             std::uint8_t* pbData, std::uint32_t dataBytes) {
    this->textureName = textureName;
    this->pbData = pbData;
    this->dataBytes = dataBytes;
}

void TexturePacket::handle(PacketListener* listener) {
    listener->handleTexture(shared_from_this());
}

void TexturePacket::read(DataInputStream* dis)  // throws IOException
{
    textureName = dis->readUTF();
    dataBytes = (std::uint32_t)dis->readShort();

    if (dataBytes > 0) {
        this->pbData = new std::uint8_t[dataBytes];

        for (std::uint32_t i = 0; i < dataBytes; i++) {
            this->pbData[i] = dis->readByte();
        }
    }
}

void TexturePacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeUTF(textureName);
    dos->writeShort((short)dataBytes);
    for (std::uint32_t i = 0; i < dataBytes; i++) {
        dos->writeByte(this->pbData[i]);
    }
}

int TexturePacket::getEstimatedSize() { return 4096; }
