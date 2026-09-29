#include "DebugOptionsPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

DebugOptionsPacket::~DebugOptionsPacket() {}

DebugOptionsPacket::DebugOptionsPacket() { m_uiVal = 0L; }

DebugOptionsPacket::DebugOptionsPacket(unsigned int uiVal) {
    this->m_uiVal = uiVal;
}

void DebugOptionsPacket::handle(PacketListener* listener) {
    listener->handleDebugOptions(shared_from_this());
}

void DebugOptionsPacket::read(DataInputStream* dis)  // throws IOException
{
    m_uiVal = (unsigned int)dis->readInt();
}

void DebugOptionsPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt((int)m_uiVal);
}

int DebugOptionsPacket::getEstimatedSize() { return sizeof(int); }
