#include "PredefComponents.h"
#include "fw/FileParser.h"
#include "Component.h"

#include <map>
#include <string>

static std::map<std::string, Component *>& PredefComponentList(bool reload = false)
{
    static std::map<std::string, Component *> list;
    static std::map<std::string, Component> components;
    if (reload) list.clear();
    if (list.empty()) {
        Component* current = nullptr;
        ParseFile("~/.evoke/packages.conf", [&current](const std::string& tag){
            components.insert(std::make_pair(tag, Component(tag)));
            current = &components.find(tag)->second;
        }, [&current, &list](const std::string& key, const std::string& value) {
            if (key == "files") {
                for (auto& f : splitWithQuotes(value)) {
                    list[f] = current;
                }
            } else if (key == "paths") {
                for (auto& path : splitWithQuotes(value)) {
                    current->pubIncl.insert(path);
                }
            } else if (key == "binary") {
                current->isBinary = true;
            } else if (key == "public_dependencies") {
                for (auto& f : splitWithQuotes(value)) {
                    if (components.count(f) == 0) {
                        components.insert(std::make_pair(f, Component(f)));
                    }
                    current->pubDeps.insert(&components.find(f)->second);
                }
            }
        });
    }

    return list;
}

void ReloadPredefComponents() 
{
  PredefComponentList(true);
}

Component *GetPredefComponent(const fs::path &path)
{
    static auto predefComponentList = PredefComponentList();
    if (reloadPredef) predefComponentList = PredefComponentList();
    if(predefComponentList.find(path.string()) != predefComponentList.end())
        return predefComponentList.find(path.string())->second;
    return nullptr;
}


