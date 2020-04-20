#pragma once

#include "Component.h"
#include "File.h"
#include "PendingCommand.h"
#include "fw/FsWatcher.hpp"

#include <fw/filesystem.hpp>
#include <ostream>
#include <string>
#include <unordered_map>

class Project
{
public:
    Project(const std::string &root);
    ~Project();
    void Reload();
    File *CreateFile(Component &c, fs::path p);
    fs::path projectRoot;
    std::unordered_map<std::string, Component> components;
    std::unordered_set<std::string> unknownHeaders;
    std::unordered_map<std::string, std::unique_ptr<File>> files;
    std::vector<PendingCommand *> buildPipeline;
    std::unordered_map<std::string, std::vector<std::string>> ambiguous;

    bool FileUpdate(fs::path changedFile, Change change);

private:
    void LoadFileList();
    bool CreateModuleMap(std::unordered_map<std::string, File *> &moduleMap);
    void MoveIncludeToImport();
    void MapImportsToModules(std::unordered_map<std::string, File *> &moduleMap);
    void CreateIncludeLookupTable(std::unordered_map<std::string, std::string> &includeLookup,
                                  std::unordered_map<std::string, std::set<std::string>> &collisions);
    void MapIncludesToDependencies(std::unordered_map<std::string, std::string> &includeLookup,
                                   std::unordered_map<std::string, std::vector<std::string>> &ambiguous);
    void PropagateExternalIncludes();
    void ExtractPublicDependencies();
    void ExtractIncludePaths();
public:
    static void ReadCodeFrom(File &f, const char *buffer, size_t buffersize);
private:
    void ReadCode(std::unordered_map<std::string, std::unique_ptr<File>> &files, const fs::path &path, Component &comp);
    bool IsItemBlacklisted(const fs::path &path);
    friend std::ostream &operator<<(std::ostream &os, const Project &p);
};

std::ostream &operator<<(std::ostream &os, const Project &p);
