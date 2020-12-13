#pragma once

#include <station.h>
#include <pipe.h>
#include <set>
#include <cmath>
#include <numeric>


class web { //граф
    friend std::ofstream& operator<<(std::ofstream&, const web&);
private:
    std::map<std::pair<size_t,size_t>, int> adj_web;
    //std::map<size_t,size_t> key_map_id; //massid, idRC
    std::set<size_t> key_map_id; //хранит id кс
    std::map<size_t,bool> used_pipes; //хранит состояние труб, к классу медленнее обращаться и не совсем их логика

    std::map<size_t,size_t> sort,colours,visited; //нужен для сортировки

    bool dfs(size_t v,size_t ts);
    bool find_neighbor(std::map<int, std::pair<double, bool>> &dkst, std::set<size_t> found, size_t id);
    bool rebuild();
    double weight(size_t id);
public:
    std::map<size_t,ITC::pipe>    pipes;
    std::map<size_t,ITC::station> stations;

    void edit(); //редактировать трубу

    bool detach(size_t r, size_t c);
    void tie_in(size_t r, size_t c, int id);

    void add_pipe(bool edit);
    void add_st(bool edit);
    void delete_pipe();
    void delete_st();
    void clean();

    void print();
    bool topological_sort();
    bool find_way(size_t from, size_t to);

    bool fout(std::string);
    bool fin(std::string);

    web();
};


std::ofstream& operator<<(std::ofstream&, const web&);
