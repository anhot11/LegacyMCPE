#pragma once

#include <memory>
#include <string>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/entity/player/Player.h"

struct STRING_VERIFY_RESPONSE;

#define MAX_SIGN_LINES 4

class SignTileEntity : public TileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_SIGNTILEENTITY; }
    static TileEntity* create() { return new SignTileEntity(); }

public:
    static const int MAX_LINE_LENGTH;

public:
    SignTileEntity();
    virtual ~SignTileEntity();
    std::string GetMessage(int i) { return m_wsmessages[i]; }
    std::string* GetMessages() { return m_wsmessages; }
    void SetMessage(int iIndex, std::string& wsText);
    int GetSelectedLine() { return m_iSelectedLine; }
    void SetSelectedLine(int iLine) { m_iSelectedLine = iLine; }
    bool IsVerified() { return m_bVerified; }
    void SetVerified(bool bVerified) { m_bVerified = bVerified; }
    bool IsCensored() { return m_bCensored; }
    void SetCensored(bool bCensored) { m_bCensored = bCensored; }

public:
private:
    std::shared_ptr<Player> playerWhoMayEdit;
    bool _isEditable;
    bool m_bVerified;
    bool m_bCensored;
    int m_iSelectedLine;

    std::string m_wsmessages[MAX_SIGN_LINES];

public:
    virtual void save(CompoundTag* tag);
    virtual void load(CompoundTag* tag);
    virtual std::shared_ptr<Packet> getUpdatePacket();
    bool isEditable();
    void setEditable(bool isEditable);
    void setAllowedPlayerEditor(std::shared_ptr<Player> player);
    std::shared_ptr<Player> getPlayerWhoMayEdit();
    virtual void setChanged();
    int handleStringVerify(STRING_VERIFY_RESPONSE* pResults);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};