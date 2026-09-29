#include "GenericStats.h"

class DamageSource;
class ItemInstance;
class Mob;
class Player;
class Stat;

GenericStats* GenericStats::instance = nullptr;

Stat* GenericStats::get_walkOneM() { return nullptr; }

Stat* GenericStats::get_swimOneM() { return nullptr; }

Stat* GenericStats::get_fallOneM() { return nullptr; }

Stat* GenericStats::get_climbOneM() { return nullptr; }

Stat* GenericStats::get_minecartOneM() { return nullptr; }

Stat* GenericStats::get_boatOneM() { return nullptr; }

Stat* GenericStats::get_pigOneM() { return nullptr; }

Stat* GenericStats::get_portalsCreated() { return nullptr; }

Stat* GenericStats::get_cowsMilked() { return nullptr; }

Stat* GenericStats::get_netherLavaCollected() { return nullptr; }

Stat* GenericStats::get_killMob() { return nullptr; }

Stat* GenericStats::get_killsZombie() { return nullptr; }

Stat* GenericStats::get_killsSkeleton() { return nullptr; }

Stat* GenericStats::get_killsCreeper() { return nullptr; }

Stat* GenericStats::get_killsSpider() { return nullptr; }

Stat* GenericStats::get_killsSpiderJockey() { return nullptr; }

Stat* GenericStats::get_killsZombiePigman() { return nullptr; }

Stat* GenericStats::get_killsSlime() { return nullptr; }

Stat* GenericStats::get_killsGhast() { return nullptr; }

Stat* GenericStats::get_killsNetherZombiePigman() { return nullptr; }

Stat* GenericStats::get_breedEntity(eINSTANCEOF entityId) { return nullptr; }

Stat* GenericStats::get_tamedEntity(eINSTANCEOF entityId) { return nullptr; }

Stat* GenericStats::get_curedEntity(eINSTANCEOF entityId) { return nullptr; }

Stat* GenericStats::get_craftedEntity(eINSTANCEOF entityId) { return nullptr; }

Stat* GenericStats::get_shearedEntity(eINSTANCEOF entityId) { return nullptr; }

Stat* GenericStats::get_totalBlocksMined() { return nullptr; }

Stat* GenericStats::get_timePlayed() { return nullptr; }

Stat* GenericStats::get_blocksPlaced(int blockId) { return nullptr; }

Stat* GenericStats::get_blocksMined(int blockId) { return nullptr; }

Stat* GenericStats::get_itemsCollected(int itemId, int itemAux) {
    return nullptr;
}

Stat* GenericStats::get_itemsCrafted(int itemId) { return nullptr; }

Stat* GenericStats::get_itemsSmelted(int itemId) {
    return this->get_itemsCrafted(itemId);
}

Stat* GenericStats::get_itemsUsed(int itemId) { return nullptr; }

Stat* GenericStats::get_itemsBought(int itemId) { return nullptr; }

Stat* GenericStats::get_killsEnderdragon() { return nullptr; }

Stat* GenericStats::get_completeTheEnd() { return nullptr; }

Stat* GenericStats::get_changedDimension(int from, int to) { return nullptr; }

Stat* GenericStats::get_enteredBiome(int biomeId) { return nullptr; }

Stat* GenericStats::get_achievement(eAward achievementId) { return nullptr; }

Stat* GenericStats::openInventory() {
    return instance->get_achievement(eAward_TakingInventory);
}

Stat* GenericStats::mineWood() {
    return instance->get_achievement(eAward_GettingWood);
}

Stat* GenericStats::buildWorkbench() {
    return instance->get_achievement(eAward_Benchmarking);
}

Stat* GenericStats::buildPickaxe() {
    return instance->get_achievement(eAward_TimeToMine);
}

Stat* GenericStats::buildFurnace() {
    return instance->get_achievement(eAward_HotTopic);
}

Stat* GenericStats::acquireIron() {
    return instance->get_achievement(eAward_AquireHardware);
}

Stat* GenericStats::buildHoe() {
    return instance->get_achievement(eAward_TimeToFarm);
}

Stat* GenericStats::makeBread() {
    return instance->get_achievement(eAward_BakeBread);
}

Stat* GenericStats::bakeCake() {
    return instance->get_achievement(eAward_TheLie);
}

Stat* GenericStats::buildBetterPickaxe() {
    return instance->get_achievement(eAward_GettingAnUpgrade);
}

Stat* GenericStats::cookFish() {
    return instance->get_achievement(eAward_DeliciousFish);
}

Stat* GenericStats::onARail() {
    return instance->get_achievement(eAward_OnARail);
}

Stat* GenericStats::buildSword() {
    return instance->get_achievement(eAward_TimeToStrike);
}

Stat* GenericStats::killEnemy() {
    return instance->get_achievement(eAward_MonsterHunter);
}

Stat* GenericStats::killCow() {
    return instance->get_achievement(eAward_CowTipper);
}

Stat* GenericStats::flyPig() {
    return instance->get_achievement(eAward_WhenPigsFly);
}

Stat* GenericStats::snipeSkeleton() {
    return instance->get_achievement(eAward_snipeSkeleton);
}

Stat* GenericStats::diamonds() {
    return instance->get_achievement(eAward_diamonds);
}

Stat* GenericStats::ghast() { return instance->get_achievement(eAward_ghast); }

Stat* GenericStats::blazeRod() {
    return instance->get_achievement(eAward_blazeRod);
}

Stat* GenericStats::potion() {
    return instance->get_achievement(eAward_potion);
}

Stat* GenericStats::theEnd() {
    return instance->get_achievement(eAward_theEnd);
}

Stat* GenericStats::winGame() {
    return instance->get_achievement(eAward_winGame);
}

Stat* GenericStats::enchantments() {
    return instance->get_achievement(eAward_enchantments);
}

Stat* GenericStats::overkill() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_overkill);
#else
    return nullptr;
#endif
}

Stat* GenericStats::bookcase() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_bookcase);
#else
    return nullptr;
#endif
}

Stat* GenericStats::leaderOfThePack() {
    return instance->get_achievement(eAward_LeaderOfThePack);
}

Stat* GenericStats::MOARTools() {
    return instance->get_achievement(eAward_MOARTools);
}

Stat* GenericStats::dispenseWithThis() {
    return instance->get_achievement(eAward_DispenseWithThis);
}

Stat* GenericStats::InToTheNether() {
    return instance->get_achievement(eAward_InToTheNether);
}

Stat* GenericStats::socialPost() {
    return instance->get_achievement(eAward_socialPost);
}

Stat* GenericStats::eatPorkChop() {
    return instance->get_achievement(eAward_eatPorkChop);
}

Stat* GenericStats::play100Days() {
    return instance->get_achievement(eAward_play100Days);
}

Stat* GenericStats::arrowKillCreeper() {
    return instance->get_achievement(eAward_arrowKillCreeper);
}

Stat* GenericStats::mine100Blocks() {
    return instance->get_achievement(eAward_mine100Blocks);
}

Stat* GenericStats::kill10Creepers() {
    return instance->get_achievement(eAward_kill10Creepers);
}

Stat* GenericStats::adventuringTime() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_adventuringTime);
#else
    return nullptr;
#endif
}

Stat* GenericStats::repopulation() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_repopulation);
#else
    return nullptr;
#endif
}

Stat* GenericStats::porkChop() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_eatPorkChop);
#else
    return nullptr;
#endif
}

Stat* GenericStats::diamondsToYou() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_diamondsToYou);
#else
    return nullptr;
#endif
}

Stat* GenericStats::passingTheTime() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_play100Days);
#else
    return nullptr;
#endif
}

Stat* GenericStats::archer() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_arrowKillCreeper);
#else
    return nullptr;
#endif
}

Stat* GenericStats::theHaggler() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_theHaggler);
#else
    return nullptr;
#endif
}

Stat* GenericStats::potPlanter() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_potPlanter);
#else
    return nullptr;
#endif
}

Stat* GenericStats::itsASign() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_itsASign);
#else
    return nullptr;
#endif
}

Stat* GenericStats::ironBelly() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_ironBelly);
#else
    return nullptr;
#endif
}

Stat* GenericStats::haveAShearfulDay() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_haveAShearfulDay);
#else
    return nullptr;
#endif
}

Stat* GenericStats::rainbowCollection() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_rainbowCollection);
#else
    return nullptr;
#endif
}

Stat* GenericStats::stayinFrosty() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_stayinFrosty);
#else
    return nullptr;
#endif
}

Stat* GenericStats::chestfulOfCobblestone() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_chestfulOfCobblestone);
#else
    return nullptr;
#endif
}

Stat* GenericStats::renewableEnergy() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_renewableEnergy);
#else
    return nullptr;
#endif
}

Stat* GenericStats::musicToMyEars() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_musicToMyEars);
#else
    return nullptr;
#endif
}

Stat* GenericStats::bodyGuard() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_bodyGuard);
#else
    return nullptr;
#endif
}

Stat* GenericStats::ironMan() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_ironMan);
#else
    return nullptr;
#endif
}

Stat* GenericStats::zombieDoctor() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_zombieDoctor);
#else
    return nullptr;
#endif
}

Stat* GenericStats::lionTamer() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->get_achievement(eAward_lionTamer);
#else
    return nullptr;
#endif
}

std::vector<uint8_t> GenericStats::getParam_walkOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_swimOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_fallOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_climbOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_minecartOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_boatOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_pigOneM(int distance) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_cowsMilked() {
    return getParam_noArgs();
}

std::vector<uint8_t> GenericStats::getParam_blocksPlaced(int id, int data,
                                                         int count) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_blocksMined(int id, int data,
                                                        int count) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_itemsCollected(int id, int aux,
                                                           int count) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_itemsCrafted(int id, int aux,
                                                         int count) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_itemsSmelted(int id, int aux,
                                                         int count) {
    return this->getParam_itemsCrafted(id, aux, count);
}

std::vector<uint8_t> GenericStats::getParam_itemsUsed(
    std::shared_ptr<Player> plr, std::shared_ptr<ItemInstance> itm) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_itemsBought(int id, int aux,
                                                        int count) {
    return getParam_noArgs();
}

std::vector<uint8_t> GenericStats::getParam_mobKill(std::shared_ptr<Player> plr,
                                                    std::shared_ptr<Mob> mob,
                                                    DamageSource* dmgSrc) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_breedEntity(eINSTANCEOF entityId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_tamedEntity(eINSTANCEOF entityId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_curedEntity(eINSTANCEOF entityId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_craftedEntity(
    eINSTANCEOF entityId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_shearedEntity(
    eINSTANCEOF entityId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_time(int timediff) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_changedDimension(int from, int to) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_enteredBiome(int biomeId) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_achievement(eAward id) {
    return getParam_noArgs();  // Really just a count on most platforms.
}

std::vector<uint8_t> GenericStats::getParam_onARail(int distance) {
    return getParam_achievement(eAward_OnARail);
}

std::vector<uint8_t> GenericStats::getParam_overkill(int damage) {
    return getParam_achievement(eAward_overkill);
}

std::vector<uint8_t> GenericStats::getParam_openInventory(int menuId) {
    return getParam_achievement(eAward_TakingInventory);
}

std::vector<uint8_t> GenericStats::getParam_chestfulOfCobblestone(
    int cobbleStone) {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return getParam_achievement(eAward_chestfulOfCobblestone);
#else
    return getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::getParam_musicToMyEars(int recordId) {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return getParam_achievement(eAward_musicToMyEars);
#else
    return getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::getParam_noArgs() {
    return std::vector<uint8_t>();  // TODO
}

std::vector<uint8_t> GenericStats::param_walk(int distance) {
    return instance->getParam_walkOneM(distance);
}

std::vector<uint8_t> GenericStats::param_swim(int distance) {
    return instance->getParam_swimOneM(distance);
}

std::vector<uint8_t> GenericStats::param_fall(int distance) {
    return instance->getParam_fallOneM(distance);
}

std::vector<uint8_t> GenericStats::param_climb(int distance) {
    return instance->getParam_climbOneM(distance);
}

std::vector<uint8_t> GenericStats::param_minecart(int distance) {
    return instance->getParam_minecartOneM(distance);
}

std::vector<uint8_t> GenericStats::param_boat(int distance) {
    return instance->getParam_boatOneM(distance);
}

std::vector<uint8_t> GenericStats::param_pig(int distance) {
    return instance->getParam_pigOneM(distance);
}

std::vector<uint8_t> GenericStats::param_cowsMilked() {
    return instance->getParam_cowsMilked();
}

std::vector<uint8_t> GenericStats::param_blocksPlaced(int id, int data,
                                                      int count) {
    return instance->getParam_blocksPlaced(id, data, count);
}

std::vector<uint8_t> GenericStats::param_blocksMined(int id, int data,
                                                     int count) {
    return instance->getParam_blocksMined(id, data, count);
}

std::vector<uint8_t> GenericStats::param_itemsCollected(int id, int aux,
                                                        int count) {
    return instance->getParam_itemsCollected(id, aux, count);
}

std::vector<uint8_t> GenericStats::param_itemsCrafted(int id, int aux,
                                                      int count) {
    return instance->getParam_itemsCrafted(id, aux, count);
}

std::vector<uint8_t> GenericStats::param_itemsSmelted(int id, int aux,
                                                      int count) {
    return instance->getParam_itemsSmelted(id, aux, count);
}

std::vector<uint8_t> GenericStats::param_itemsUsed(
    std::shared_ptr<Player> plr, std::shared_ptr<ItemInstance> itm) {
    if ((plr != nullptr) && (itm != nullptr))
        return instance->getParam_itemsUsed(plr, itm);
    else
        return instance->getParam_noArgs();
}

std::vector<uint8_t> GenericStats::param_itemsBought(int id, int aux,
                                                     int count) {
    return instance->getParam_itemsBought(id, aux, count);
}

std::vector<uint8_t> GenericStats::param_mobKill(std::shared_ptr<Player> plr,
                                                 std::shared_ptr<Mob> mob,
                                                 DamageSource* dmgSrc) {
    if ((plr != nullptr) && (mob != nullptr))
        return instance->getParam_mobKill(plr, mob, dmgSrc);
    else
        return instance->getParam_noArgs();
}

std::vector<uint8_t> GenericStats::param_breedEntity(eINSTANCEOF entityId) {
    return instance->getParam_breedEntity(entityId);
}

std::vector<uint8_t> GenericStats::param_tamedEntity(eINSTANCEOF entityId) {
    return instance->getParam_tamedEntity(entityId);
}

std::vector<uint8_t> GenericStats::param_curedEntity(eINSTANCEOF entityId) {
    return instance->getParam_curedEntity(entityId);
}

std::vector<uint8_t> GenericStats::param_craftedEntity(eINSTANCEOF entityId) {
    return instance->getParam_craftedEntity(entityId);
}

std::vector<uint8_t> GenericStats::param_shearedEntity(eINSTANCEOF entityId) {
    return instance->getParam_shearedEntity(entityId);
}

std::vector<uint8_t> GenericStats::param_time(int timediff) {
    return instance->getParam_time(timediff);
}

std::vector<uint8_t> GenericStats::param_changedDimension(int from, int to) {
    return instance->getParam_changedDimension(from, to);
}

std::vector<uint8_t> GenericStats::param_enteredBiome(int biomeId) {
    return instance->getParam_enteredBiome(biomeId);
}

std::vector<uint8_t> GenericStats::param_noArgs() {
    return instance->getParam_noArgs();
}

std::vector<uint8_t> GenericStats::param_openInventory() {
    return instance->getParam_achievement(eAward_TakingInventory);
}

std::vector<uint8_t> GenericStats::param_mineWood() {
    return instance->getParam_achievement(eAward_GettingWood);
}

std::vector<uint8_t> GenericStats::param_buildWorkbench() {
    return instance->getParam_achievement(eAward_Benchmarking);
}

std::vector<uint8_t> GenericStats::param_buildPickaxe() {
    return instance->getParam_achievement(eAward_TimeToMine);
}

std::vector<uint8_t> GenericStats::param_buildFurnace() {
    return instance->getParam_achievement(eAward_HotTopic);
}

std::vector<uint8_t> GenericStats::param_acquireIron() {
    return instance->getParam_achievement(eAward_AquireHardware);
}

std::vector<uint8_t> GenericStats::param_buildHoe() {
    return instance->getParam_achievement(eAward_TimeToFarm);
}

std::vector<uint8_t> GenericStats::param_makeBread() {
    return instance->getParam_achievement(eAward_BakeBread);
}

std::vector<uint8_t> GenericStats::param_bakeCake() {
    return instance->getParam_achievement(eAward_TheLie);
}

std::vector<uint8_t> GenericStats::param_buildBetterPickaxe() {
    return instance->getParam_achievement(eAward_GettingAnUpgrade);
}

std::vector<uint8_t> GenericStats::param_cookFish() {
    return instance->getParam_achievement(eAward_DeliciousFish);
}

std::vector<uint8_t> GenericStats::param_onARail(int distance) {
    return instance->getParam_onARail(distance);
}

std::vector<uint8_t> GenericStats::param_buildSword() {
    return instance->getParam_achievement(eAward_TimeToStrike);
}

std::vector<uint8_t> GenericStats::param_killEnemy() {
    return instance->getParam_achievement(eAward_MonsterHunter);
}

std::vector<uint8_t> GenericStats::param_killCow() {
    return instance->getParam_achievement(eAward_CowTipper);
}

std::vector<uint8_t> GenericStats::param_flyPig() {
    return instance->getParam_achievement(eAward_WhenPigsFly);
}

std::vector<uint8_t> GenericStats::param_snipeSkeleton() {
    return instance->getParam_achievement(eAward_snipeSkeleton);
}

std::vector<uint8_t> GenericStats::param_diamonds() {
    return instance->getParam_achievement(eAward_diamonds);
}

std::vector<uint8_t> GenericStats::param_ghast() {
    return instance->getParam_achievement(eAward_ghast);
}

std::vector<uint8_t> GenericStats::param_blazeRod() {
    return instance->getParam_achievement(eAward_blazeRod);
}

std::vector<uint8_t> GenericStats::param_potion() {
    return instance->getParam_achievement(eAward_potion);
}

std::vector<uint8_t> GenericStats::param_theEnd() {
    return instance->getParam_achievement(eAward_theEnd);
}

std::vector<uint8_t> GenericStats::param_winGame() {
    return instance->getParam_achievement(eAward_winGame);
}

std::vector<uint8_t> GenericStats::param_enchantments() {
    return instance->getParam_achievement(eAward_enchantments);
}

std::vector<uint8_t> GenericStats::param_overkill(int dmg) {
    return instance->getParam_overkill(dmg);
}

std::vector<uint8_t> GenericStats::param_bookcase() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_bookcase);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_leaderOfThePack() {
    return instance->getParam_achievement(eAward_LeaderOfThePack);
}

std::vector<uint8_t> GenericStats::param_MOARTools() {
    return instance->getParam_achievement(eAward_MOARTools);
}

std::vector<uint8_t> GenericStats::param_dispenseWithThis() {
    return instance->getParam_achievement(eAward_DispenseWithThis);
}

std::vector<uint8_t> GenericStats::param_InToTheNether() {
    return instance->getParam_achievement(eAward_InToTheNether);
}

std::vector<uint8_t> GenericStats::param_socialPost() {
    return instance->getParam_achievement(eAward_socialPost);
}

std::vector<uint8_t> GenericStats::param_eatPorkChop() {
    return instance->getParam_achievement(eAward_eatPorkChop);
}

std::vector<uint8_t> GenericStats::param_play100Days() {
    return instance->getParam_achievement(eAward_play100Days);
}

std::vector<uint8_t> GenericStats::param_arrowKillCreeper() {
    return instance->getParam_achievement(eAward_arrowKillCreeper);
}

std::vector<uint8_t> GenericStats::param_mine100Blocks() {
    return instance->getParam_achievement(eAward_mine100Blocks);
}

std::vector<uint8_t> GenericStats::param_kill10Creepers() {
    return instance->getParam_achievement(eAward_kill10Creepers);
}

std::vector<uint8_t> GenericStats::param_adventuringTime() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_adventuringTime);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_repopulation() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_repopulation);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_porkChop() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_eatPorkChop);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_diamondsToYou() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_diamondsToYou);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_passingTheTime() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_play100Days);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_archer() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_arrowKillCreeper);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_theHaggler() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_theHaggler);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_potPlanter() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_potPlanter);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_itsASign() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_itsASign);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_ironBelly() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_ironBelly);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_haveAShearfulDay() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_haveAShearfulDay);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_rainbowCollection() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_rainbowCollection);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_stayinFrosty() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_stayinFrosty);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_chestfulOfCobblestone(
    int cobbleStone) {
    return instance->getParam_chestfulOfCobblestone(cobbleStone);
}

std::vector<uint8_t> GenericStats::param_renewableEnergy() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_renewableEnergy);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_musicToMyEars(int recordId) {
    return instance->getParam_musicToMyEars(recordId);
}

std::vector<uint8_t> GenericStats::param_bodyGuard() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_bodyGuard);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_ironMan() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_ironMan);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_zombieDoctor() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_zombieDoctor);
#else
    return instance->getParam_noArgs();
#endif
}

std::vector<uint8_t> GenericStats::param_lionTamer() {
#if defined(_EXTENDED_ACHIEVEMENTS)
    return instance->getParam_achievement(eAward_lionTamer);
#else
    return instance->getParam_noArgs();
#endif
}
