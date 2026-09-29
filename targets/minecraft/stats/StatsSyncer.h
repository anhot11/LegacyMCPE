#pragma once
#include <string>
#include <unordered_map>

class File;
class StatsCounter;
class User;
class File;
class Stat;

class StatsSyncher {
private:
    static const int SAVE_INTERVAL = 20 * 5;
    static const int SEND_INTERVAL = 20 * 60;

    volatile bool busy;

    volatile std::unordered_map<Stat*, int>* serverStats;
    volatile std::unordered_map<Stat*, int>* failedSentStats;

    StatsCounter* statsCounter;
    File *unsentFile, *lastServerFile;
    File *unsentFileTmp, *lastServerFileTmp;
    File *unsentFileOld, *lastServerFileOld;
    User* user;

    int noSaveIn, noSendIn;

public:
    StatsSyncher(User* user, StatsCounter* statsCounter, File* dir);

private:
    void attemptRename(File* dir, const std::string& name, File* to);
    std::unordered_map<Stat*, int>* loadStatsFromDisk(File* file, File* tmp,
                                                      File* old);
    std::unordered_map<Stat*, int>* loadStatsFromDisk(File* file);
    void doSend(std::unordered_map<Stat*, int>* stats);
    void doSave(std::unordered_map<Stat*, int>* stats, File* file, File* tmp,
                File* old);

protected:
    std::unordered_map<Stat*, int>* doGetStats();

public:
    void getStatsFromServer();
    void saveUnsent(std::unordered_map<Stat*, int>* stats);
    void sendUnsent(std::unordered_map<Stat*, int>* stats,
                    std::unordered_map<Stat*, int>* fullStats);
    void forceSendUnsent(std::unordered_map<Stat*, int>* stats);
    void forceSaveUnsent(std::unordered_map<Stat*, int>* stats);
    bool maySave();
    bool maySend();
    void tick();
};
