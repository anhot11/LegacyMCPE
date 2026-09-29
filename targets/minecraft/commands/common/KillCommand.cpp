#include "KillCommand.h"

#include <limits>
#include <memory>
#include <string>

#include "minecraft/commands/CommandSender.h"
#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/player/Player.h"

EGameCommand KillCommand::getId() { return eGameCommand_Kill; }

int KillCommand::getPermissionLevel() { return LEVEL_ALL; }

void KillCommand::execute(std::shared_ptr<CommandSender> source,
                          std::vector<uint8_t>& commandData) {
    std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(source);

    player->hurt(DamageSource::outOfWorld, std::numeric_limits<float>::max());

    source->sendMessage("Ouch. That look like it hurt.");
    // source.sendMessage(ChatMessageComponent.forTranslation("commands.kill.success"));
}
