#include <pipe.h>
using namespace ITC;
using namespace std;

size_t pipe::sId = 0;

pipe::pipe():id(sId++){
    name="p"+to_string(id);
    length = 100;
    diameter = 1400; //mm
}


pipe::pipe(bool change):pipe::pipe(){
    if (!change) return;
    else {
        name = check_input_str("name of pipe('ctrl+z' to end input)");
        length=check_input("length");
        diameter=check_input_int("diameter");
        under_repair = check_ans("under repair?");
        return;
    }
}

pipe::pipe(std::ifstream& ifs, size_t tid):id(tid){
    using namespace std;
    string str;

    getline(ifs, str, '|');
    getline(ifs, name, '|');

    getline(ifs, str, '|');
    length=stod(str);

    getline(ifs, str, '|');
    diameter=stoi(str);

    getline(ifs, str, '|');
    under_repair=str=="1";

    sId=id+1; //меняется по последнему только в случае считывания из файла
    return;
}


void pipe::change(){
    name = check_input_str("name of pipe('ctrl+z' to end input)");
    under_repair = check_ans("under repair?");
    return;
}


size_t pipe::get_id() const{
    return id;
}

size_t pipe::get_max_id(){
    return pipe::sId;
}

void pipe::kill_sId(){
    pipe::sId = 0;
    return;
}

pipe::~pipe(){
    //std::cout<<"pipe id"<<id<<" is destructured"<<std::endl;
}

std::ostream& operator<<(std::ostream& os, const pipe& mypipe){
    os << "\npipe id" << mypipe.get_id()
       << " 'called "+ mypipe.name
       << "\n"<<(mypipe.under_repair ? "is":"is not")
       << " under repair\nlength:\t\t" << mypipe.length
       << "\ndiameter:\t"<< mypipe.diameter << "\n\n";

    return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const pipe& mypipe){
    string ans = "P"
                +to_string(mypipe.get_id())+"|"
                +mypipe.name+"|"
                +to_string(mypipe.length)+"|"
                +to_string(mypipe.diameter)+"|"
                +(mypipe.under_repair? '1':'0')+"|\n";
    ofs << ans;
    return ofs;
}
