#include <station.h>
using namespace ITC;
using namespace std;

size_t station::sId = 0;


station::station():id(sId++){
    name="s"+to_string(id);
    quantity = 200;
    quantity_in_work = 193;
    efficiency = double(quantity_in_work)/quantity*100;
    return;
}

station::station(bool change):station::station(){
    if (!change) return;
    else {
        name = check_input_str("name of station('ctrl+z' to end input)");

        quantity = check_input_int("quantity of departments");
        quantity_in_work = check_input_int("quantity of departments in work");

        while(quantity < quantity_in_work) {
            cout<<"ERROR: to many departments in work, please reenter\n";
            quantity = check_input_int("quantity of departments");
            quantity_in_work = check_input_int("quantity of departments in work");
        }
        efficiency = double(quantity_in_work)/quantity*100;
    }
    return;
}

station::station(std::ifstream& ifs, size_t tid):id(tid){
    string str;

    getline(ifs, str, '|');
    getline(ifs, name, '|');

    getline(ifs, str, '|');
    efficiency=stoi(str);

    getline(ifs, str, '|');
    quantity=stoi(str);

    getline(ifs, str, '|');
    quantity_in_work=stoi(str);

    sId=id+1; //меняется по последнему только в случае считывания из файла

    return;
}


size_t station::get_id() const{
    return id;
}

size_t station::get_max_id(){
    return station::sId;
}

void station::kill_sId(){
    station::sId = 0;
    return;
}


void station::change(){
    name = check_input_str("name of station('ctrl+z' to end input)");

    quantity = check_input_int("quantity of departments");
    quantity_in_work = check_input_int("quantity of departments in work");

    while(quantity < quantity_in_work) {
        cout<<"ERROR: to many departments in work, please reenter\n";
        quantity = check_input_int("quantity of departments");
        quantity_in_work = check_input_int("quantity of departments in work");
    }
    efficiency = double(quantity_in_work)/quantity*100;
    return;
}


station::~station(){
    //std::cout<<"station id"<<id<<" is destructured"<<std::endl;
}


std::ostream& operator<<(std::ostream& os, const station& my_st){
    os << "\nstation id" << my_st.get_id()
       << " 'called "+ my_st.name
       << "\nefficiency score:\t" << my_st.efficiency
       << "\nall stations count:\t" << my_st.quantity
       << "\nstations in work:\t"<< my_st.quantity_in_work << "\n\n";

    return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const station& my_st){
    ofs <<  "S"
            +to_string(my_st.get_id())+"|"
            +my_st.name+"|"
            +to_string(my_st.efficiency)+"|"
            << my_st.quantity << "|"
            << my_st.quantity_in_work << "|\n";

    return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, ITC::station& my_st){
    string str;

    getline(ifs, str, '|');//взяли id первого

    getline(ifs, my_st.name, '|');//дали имя

    getline(ifs, str, '|');//эффективность
    my_st.efficiency=stoi(str);

    getline(ifs, str, '|');//кол-во станций
    my_st.quantity=stoi(str);

    getline(ifs, str, '|');//кол-во станций в работе
    my_st.quantity_in_work=stoi(str);

    return ifs;
}
