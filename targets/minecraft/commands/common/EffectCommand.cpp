
#include "minecraft/commands/common/EffectCommand.h"

#include <stdint.h>

#include <memory>
#include <string>

#include "minecraft/commands/CommandsEnum.h"

class CommandSender;

EGameCommand EffectCommand::getId() { return eGameCommand_Effect; }

int EffectCommand::getPermissionLevel() { return LEVEL_GAMEMASTERS; }

std::string EffectCommand::getUsage(CommandSender* source) {
    return "commands.effect.usage";
}

void EffectCommand::execute(std::shared_ptr<CommandSender> source,
                            std::vector<uint8_t>& commandData) {
    // if (args.size() >= 2)
    //{
    //	Player player = convertToPlayer(source, args[0]);

    //	if (args[1].equals("clear")) {
    //		if (player.getActiveEffects().isEmpty()) {
    //			throw new
    // CommandException("commands.effect.failure.notActive.all",
    // player.getAName()); 		} else {
    // player.removeAllEffects(); 			logAdminAction(source,
    //"commands.effect.success.removed.all", player.getAName());
    //		}
    //	} else {
    //		int effectId = convertArgToInt(source, args[1], 1);
    //		int duration = SharedConstants.TICKS_PER_SECOND * 30;
    //		int seconds = 30;
    //		int amplifier = 0;

    //		if (effectId < 0 || effectId >= MobEffect.effects.size() ||
    // MobEffect.effects[effectId] == null) { 			throw new
    // InvalidNumberException("commands.effect.notFound", effectId);
    //		}

    //		if (args.size() >= 3) {
    //			seconds = convertArgToInt(source, args[2], 0, 1000000);
    //			if (MobEffect.effects[effectId].isInstantenous()) {
    //				duration = seconds;
    //			} else {
    //				duration = seconds *
    // SharedConstants.TICKS_PER_SECOND;
    //			}
    //		} else if (MobEffect.effects[effectId].isInstantenous()) {
    //			duration = 1;
    //		}

    //		if (args.size() >= 4) {
    //			amplifier = convertArgToInt(source, args[3], 0, 255);
    //		}

    //		if (seconds == 0) {
    //			if (player.hasEffect(effectId)) {
    //				player.removeEffect(effectId);
    //				logAdminAction(source,
    //"commands.effect.success.removed",
    // ChatMessageComponent.forTranslation(MobEffect.effects[effectId].getDescriptionId()),
    // player.getAName()); 			} else {
    // throw new CommandException("commands.effect.failure.notActive",
    // ChatMessageComponent.forTranslation(MobEffect.effects[effectId].getDescriptionId()),
    // player.getAName());
    //			}
    //		} else {
    //			MobEffectInstance instance = new
    // MobEffectInstance(effectId, duration, amplifier);
    //			player.addEffect(instance);
    //			logAdminAction(source, "commands.effect.success",
    // ChatMessageComponent.forTranslation(instance.getDescriptionId()),
    // effectId, amplifier, player.getAName(), seconds);
    //		}
    //	}

    //	return;
    //}

    // throw new UsageException("commands.effect.usage");
}

std::string EffectCommand::getPlayerNames() {
    return "";  // MinecraftServer::getInstance()->getPlayerNames();
}

bool EffectCommand::isValidWildcardPlayerArgument(std::string args,
                                                  int argumentIndex) {
    return argumentIndex == 0;
}