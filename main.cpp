#include <mymenu.h>
using namespace std;
using namespace ITC;

int main(){
    web web;
    while(1){
        switch(menu()){
        case 0:
            return 0;
        case 1:
            web.add_pipe(false);
            break;
        case 2:
            web.add_st(false);
            break;
        case 3: //see all
            for (auto v: web.pipes)    cout << v.second;
            for (auto v: web.stations) cout << v.second;
            break;
        case 4: //edit pipe
            if(!edit(web.pipes,check_input_int("ID"))) cout << "can't edit pipe\n";
            break;
        case 5: //edit station
            if(!edit(web.stations,check_input_int("ID"))) cout << "can't edit station\n";
            break;
        case 6: //select pipes
            if(!selectPipes(web.pipes)) cout<<"can't select any pipe\n";
            break;
        case 7: //select stations
            if(!selectStations(web.stations)) cout<<"can't select any station\n";
            break;
        case 8: //save
            web.fout(check_input_str("name of file")+".itc");
            break;
        case 9: //load
            web.fin(check_input_str("name of file")+".itc");
            break;
        case 10: //delete pipe
            web.delete_pipe();
            break;
        case 11: //delete station
            web.delete_st();
            break;
        case 12: //clean web
            web.clean();
            break;
        case 13: //view web
            web.print();
            break;
        case 14: //edit web
            web.edit();
            break;
        case 15:
            web.topological_sort();
            break;
        }
    }
}
