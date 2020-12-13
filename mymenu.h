#pragma once

#include <web.h>

namespace ITC {
int menu();
bool fin(std::string, std::map<size_t,pipe>&, std::map<size_t,station>&);
bool fout(std::string, std::map<size_t, pipe>&, std::map<size_t, station>&);
bool selectPipes(std::map<size_t,pipe>&);
bool selectStations(std::map<size_t,station>&);
std::vector<size_t> filterSelectPipes(std::map<size_t,pipe>&);
std::vector<size_t> filterSelectStations(std::map<size_t,station>&);


//фильтр для поиска
template<typename T, typename Class>
using filter = bool(*)(const Class&, T);

//варинаты фильтра
template<typename Class>
bool checkByName(const Class &p, std::string name){
    return p.name == name;
}

bool checkByStatus(const ITC::pipe &p, bool state);

bool checkByEffcy(const ITC::station &s, int effcy);

//template<typename Class>
//bool checkByID(const Class &p, int id){
//    return p.get_id() == id;
//}

//сам поиск
template<typename T, typename Class>
std::vector<size_t> FindByFilter(std::map<size_t,Class>& ps, filter<T,Class> f, T param){
    std::vector<size_t> ans;
    for(auto k: ps){
        if(f(k.second,param)) ans.emplace_back(k.first);
    }
    return ans;
}

template<typename Class>
bool edit(std::map<size_t,Class>& ps, int id){         
    if (ps.find(id) != ps.end()) {
        ps[id].change();
        return false;
    } else {
        std::cout << "ID not found\n";
        return true;
    }
}

}
