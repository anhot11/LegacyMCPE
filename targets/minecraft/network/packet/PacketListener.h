#pragma once

#include <memory>

#include "DisconnectPacket.h"
#include "minecraft/network/packet/DisconnectPacket.h"

class Packet;
class AddEntityPacket;
class AddGlobalEntityPacket;
class AddMobPacket;
class AddPaintingPacket;
class AddPlayerPacket;
class AnimatePacket;
class AwardStatPacket;
class BlockRegionUpdatePacket;
class ChatPacket;
class ChunkTilesUpdatePacket;
class ChunkVisibilityPacket;
class ComplexItemDataPacket;
class ContainerAckPacket;
class ContainerClickPacket;
class ContainerClosePacket;
class ContainerOpenPacket;
class ContainerSetContentPacket;
class ContainerSetDataPacket;
class ContainerSetSlotPacket;
class EntityActionAtPositionPacket;
class EntityEventPacket;
class ExplodePacket;
class GameEventPacket;
class InteractPacket;
class KeepAlivePacket;
class LevelEventPacket;
class LoginPacket;
class MoveEntityPacket;
class MoveEntityPacketSmall;
class MovePlayerPacket;
class PlayerActionPacket;
class PlayerCommandPacket;
class PlayerInputPacket;
class PreLoginPacket;
class RemoveEntitiesPacket;
class RespawnPacket;
class SetCarriedItemPacket;
class SetEntityDataPacket;
class SetEntityMotionPacket;
class SetEquippedItemPacket;
class SetHealthPacket;
class SetEntityLinkPacket;
class SetSpawnPositionPacket;
class SetTimePacket;
class SignUpdatePacket;
class TakeItemEntityPacket;
class TeleportEntityPacket;
class TileEventPacket;
class TileUpdatePacket;
class UseItemPacket;
// 1.8.2
class GetInfoPacket;
class UpdateMobEffectPacket;
class RemoveMobEffectPacket;
class PlayerInfoPacket;
class SetExperiencePacket;
class SetCreativeModeSlotPacket;
class AddExperienceOrbPacket;
// 1.0.1
class ContainerButtonClickPacket;
class TileEntityDataPacket;
// 1.1
class CustomPayloadPacket;
// 1.2.3
class RotateHeadPacket;
// 1.3.2
class ClientProtocolPacket;
class ServerAuthDataPacket;
class SharedKeyPacket;
class PlayerAbilitiesPacket;
class ChatAutoCompletePacket;
class ClientInformationPacket;
class LevelSoundPacket;
class TileDestructionPacket;
class ClientCommandPacket;
class LevelChunksPacket;
// 1.6.4
class SetObjectivePacket;
class SetScorePacket;
class SetDisplayObjectivePacket;
class SetPlayerTeamPacket;
class LevelParticlesPacket;
class UpdateAttributesPacket;
class TileEditorOpenPacket;
// 4J Added
class CraftItemPacket;
class TradeItemPacket;
class DebugOptionsPacket;
class ServerSettingsChangedPacket;
class TexturePacket;
class TextureAndGeometryPacket;
class ChunkVisibilityAreaPacket;
class UpdateProgressPacket;
class TextureChangePacket;
class TextureAndGeometryChangePacket;
class UpdateGameRuleProgressPacket;
class KickPlayerPacket;
class AdditionalModelPartsPacket;
class XZPacket;
class GameCommandPacket;

class PacketListener {
public:
    virtual bool isServerPacketListener() = 0;
    virtual void handleBlockRegionUpdate(
        std::shared_ptr<BlockRegionUpdatePacket> packet);
    virtual void onUnhandledPacket(std::shared_ptr<Packet> packet);
    virtual void onDisconnect(DisconnectPacket::eDisconnectReason reason,
                              void* reasonObjects);
    virtual void handleDisconnect(std::shared_ptr<DisconnectPacket> packet);
    virtual void handleLogin(std::shared_ptr<LoginPacket> packet);
    virtual void handleMovePlayer(std::shared_ptr<MovePlayerPacket> packet);
    virtual void handleChunkTilesUpdate(
        std::shared_ptr<ChunkTilesUpdatePacket> packet);
    virtual void handlePlayerAction(std::shared_ptr<PlayerActionPacket> packet);
    virtual void handleTileUpdate(std::shared_ptr<TileUpdatePacket> packet);
    virtual void handleChunkVisibility(
        std::shared_ptr<ChunkVisibilityPacket> packet);
    virtual void handleAddPlayer(std::shared_ptr<AddPlayerPacket> packet);
    virtual void handleMoveEntity(std::shared_ptr<MoveEntityPacket> packet);
    virtual void handleMoveEntitySmall(
        std::shared_ptr<MoveEntityPacketSmall> packet);
    virtual void handleTeleportEntity(
        std::shared_ptr<TeleportEntityPacket> packet);
    virtual void handleUseItem(std::shared_ptr<UseItemPacket> packet);
    virtual void handleSetCarriedItem(
        std::shared_ptr<SetCarriedItemPacket> packet);
    virtual void handleRemoveEntity(
        std::shared_ptr<RemoveEntitiesPacket> packet);
    virtual void handleTakeItemEntity(
        std::shared_ptr<TakeItemEntityPacket> packet);
    virtual void handleChat(std::shared_ptr<ChatPacket> packet);
    virtual void handleAddEntity(std::shared_ptr<AddEntityPacket> packet);
    virtual void handleAnimate(std::shared_ptr<AnimatePacket> packet);
    virtual void handlePlayerCommand(
        std::shared_ptr<PlayerCommandPacket> packet);
    virtual void handlePreLogin(std::shared_ptr<PreLoginPacket> packet);
    virtual void handleAddMob(std::shared_ptr<AddMobPacket> packet);
    virtual void handleSetTime(std::shared_ptr<SetTimePacket> packet);
    virtual void handleSetSpawn(std::shared_ptr<SetSpawnPositionPacket> packet);
    virtual void handleSetEntityMotion(
        std::shared_ptr<SetEntityMotionPacket> packet);
    virtual void handleSetEntityData(
        std::shared_ptr<SetEntityDataPacket> packet);
    virtual void handleEntityLinkPacket(
        std::shared_ptr<SetEntityLinkPacket> packet);
    virtual void handleInteract(std::shared_ptr<InteractPacket> packet);
    virtual void handleEntityEvent(std::shared_ptr<EntityEventPacket> packet);
    virtual void handleSetHealth(std::shared_ptr<SetHealthPacket> packet);
    virtual void handleRespawn(std::shared_ptr<RespawnPacket> packet);
    virtual void handleExplosion(std::shared_ptr<ExplodePacket> packet);
    virtual void handleContainerOpen(
        std::shared_ptr<ContainerOpenPacket> packet);
    virtual void handleContainerClose(
        std::shared_ptr<ContainerClosePacket> packet);
    virtual void handleContainerClick(
        std::shared_ptr<ContainerClickPacket> packet);
    virtual void handleContainerSetSlot(
        std::shared_ptr<ContainerSetSlotPacket> packet);
    virtual void handleContainerContent(
        std::shared_ptr<ContainerSetContentPacket> packet);
    virtual void handleSignUpdate(std::shared_ptr<SignUpdatePacket> packet);
    virtual void handleContainerSetData(
        std::shared_ptr<ContainerSetDataPacket> packet);
    virtual void handleSetEquippedItem(
        std::shared_ptr<SetEquippedItemPacket> packet);
    virtual void handleContainerAck(std::shared_ptr<ContainerAckPacket> packet);
    virtual void handleAddPainting(std::shared_ptr<AddPaintingPacket> packet);
    virtual void handleTileEvent(std::shared_ptr<TileEventPacket> packet);
    virtual void handleAwardStat(std::shared_ptr<AwardStatPacket> packet);
    virtual void handleEntityActionAtPosition(
        std::shared_ptr<EntityActionAtPositionPacket> packet);
    virtual void handlePlayerInput(std::shared_ptr<PlayerInputPacket> packet);
    virtual void handleGameEvent(std::shared_ptr<GameEventPacket> packet);
    virtual void handleAddGlobalEntity(
        std::shared_ptr<AddGlobalEntityPacket> packet);
    virtual void handleComplexItemData(
        std::shared_ptr<ComplexItemDataPacket> packet);
    virtual void handleLevelEvent(std::shared_ptr<LevelEventPacket> packet);

    //  1.8.2
    virtual void handleGetInfo(std::shared_ptr<GetInfoPacket> packet);
    virtual void handleUpdateMobEffect(
        std::shared_ptr<UpdateMobEffectPacket> packet);
    virtual void handleRemoveMobEffect(
        std::shared_ptr<RemoveMobEffectPacket> packet);
    virtual void handlePlayerInfo(std::shared_ptr<PlayerInfoPacket> packet);
    virtual void handleKeepAlive(std::shared_ptr<KeepAlivePacket> packet);
    virtual void handleSetExperience(
        std::shared_ptr<SetExperiencePacket> packet);
    virtual void handleSetCreativeModeSlot(
        std::shared_ptr<SetCreativeModeSlotPacket> packet);
    virtual void handleAddExperienceOrb(
        std::shared_ptr<AddExperienceOrbPacket> packet);

    // 1.0.1
    virtual void handleContainerButtonClick(
        std::shared_ptr<ContainerButtonClickPacket> packet);
    virtual void handleTileEntityData(
        std::shared_ptr<TileEntityDataPacket> tileEntityDataPacket);

    // 1.1s
    virtual void handleCustomPayload(
        std::shared_ptr<CustomPayloadPacket> customPayloadPacket);

    // 1.2.3
    virtual void handleRotateMob(
        std::shared_ptr<RotateHeadPacket> rotateMobPacket);

    // 1.3.2
    virtual void handleClientProtocolPacket(
        std::shared_ptr<ClientProtocolPacket> packet);
    virtual void handleServerAuthData(
        std::shared_ptr<ServerAuthDataPacket> packet);
    // virtual void handleSharedKey(std::shared_ptr<SharedKeyPacket> packet);
    virtual void handlePlayerAbilities(
        std::shared_ptr<PlayerAbilitiesPacket> playerAbilitiesPacket);
    virtual void handleChatAutoComplete(
        std::shared_ptr<ChatAutoCompletePacket> packet);
    virtual void handleClientInformation(
        std::shared_ptr<ClientInformationPacket> packet);
    virtual void handleSoundEvent(std::shared_ptr<LevelSoundPacket> packet);
    virtual void handleTileDestruction(
        std::shared_ptr<TileDestructionPacket> packet);
    virtual void handleClientCommand(
        std::shared_ptr<ClientCommandPacket> packet);
    // virtual void handleLevelChunks(std::shared_ptr<LevelChunksPacket>
    // packet);
    virtual bool canHandleAsyncPackets();

    // 1.6.4
    virtual void handleAddObjective(std::shared_ptr<SetObjectivePacket> packet);
    virtual void handleSetScore(std::shared_ptr<SetScorePacket> packet);
    virtual void handleSetDisplayObjective(
        std::shared_ptr<SetDisplayObjectivePacket> packet);
    virtual void handleSetPlayerTeamPacket(
        std::shared_ptr<SetPlayerTeamPacket> packet);
    virtual void handleParticleEvent(
        std::shared_ptr<LevelParticlesPacket> packet);
    virtual void handleUpdateAttributes(
        std::shared_ptr<UpdateAttributesPacket> packet);
    virtual void handleTileEditorOpen(
        std::shared_ptr<TileEditorOpenPacket> tileEditorOpenPacket);
    virtual bool isDisconnected();

    // 4J Added
    virtual void handleCraftItem(std::shared_ptr<CraftItemPacket> packet);
    virtual void handleTradeItem(std::shared_ptr<TradeItemPacket> packet);
    virtual void handleDebugOptions(std::shared_ptr<DebugOptionsPacket> packet);
    virtual void handleServerSettingsChanged(
        std::shared_ptr<ServerSettingsChangedPacket> packet);
    virtual void handleTexture(std::shared_ptr<TexturePacket> packet);
    virtual void handleTextureAndGeometry(
        std::shared_ptr<TextureAndGeometryPacket> packet);
    virtual void handleChunkVisibilityArea(
        std::shared_ptr<ChunkVisibilityAreaPacket> packet);
    virtual void handleUpdateProgress(
        std::shared_ptr<UpdateProgressPacket> packet);
    virtual void handleTextureChange(
        std::shared_ptr<TextureChangePacket> packet);
    virtual void handleTextureAndGeometryChange(
        std::shared_ptr<TextureAndGeometryChangePacket> packet);
    virtual void handleUpdateGameRuleProgressPacket(
        std::shared_ptr<UpdateGameRuleProgressPacket> packet);
    virtual void handleKickPlayer(std::shared_ptr<KickPlayerPacket> packet);
    virtual void handleXZ(std::shared_ptr<XZPacket> packet);
    virtual void handleGameCommand(std::shared_ptr<GameCommandPacket> packet);
};
