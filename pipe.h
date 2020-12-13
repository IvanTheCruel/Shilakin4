#pragma once

#include <check_input.h>

class ITC::pipe
{
private:
    static size_t sId;
    size_t id;

public:
    std::string name;
    double length;
    int diameter;
    bool under_repair = false;

    size_t get_id() const;
    static size_t get_max_id();
    static void kill_sId();
    void change();

    pipe();
    pipe(bool);
    pipe(std::ifstream&, size_t);
    ~pipe();
};


std::ostream& operator<<(std::ostream&, const ITC::pipe&);
std::ofstream& operator<<(std::ofstream&, const ITC::pipe&);
