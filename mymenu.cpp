#include <mymenu.h>
using namespace std;

int ITC::menu(){
    vector<string> q = {"1-add pipe","2-add station","3-see all",
                        "4-edit pipe","5-edit station","6-select pipes",
                        "7-select stations","8-save","9-load",
                        "10-delete pipe","11-delete station",
                        "12-rebuild web","13-view web","14-edit web",
                        "15-topological sort","0-exit"};
    cout << "options:\n";
    for (auto t: q){
        cout << t << "\n";
    }
    return check_input_int("option:");
}

bool ITC::checkByStatus(const ITC::pipe &p, bool state){
    return p.under_repair == state;
}

bool ITC::checkByEffcy(const ITC::station &s, int effcy){
    return s.efficiency == effcy;
}


std::vector<size_t> ITC::filterSelectPipes(std::map<size_t,pipe> &ps){
    cout << "options: 1-select by name, 2-select by state,3-both\n";
    std::vector<size_t> find;
    switch (check_input_int("option")){
    case 1:
        return FindByFilter(ps,checkByName,check_input_str("name of station('ctrl+z' to end input)"));
    case 2:
        return FindByFilter(ps,checkByStatus,check_ans("under repair?"));
    case 3:
        for     (auto temp_by_name:  FindByFilter(ps,checkByName,check_input_str("name of station('ctrl+z' to end input)"))){
            for (auto temp_by_state: FindByFilter(ps,checkByStatus,check_ans("under repair?"))){
                if (temp_by_name==temp_by_state)
                    find.emplace_back(temp_by_name);
            }
        }
        break;
    default:
        break;
    }
    return find;
}

std::vector<size_t> ITC::filterSelectStations(std::map<size_t,station>& ps){
    cout << "options: 1-select by name, 2-select by efficiency,3-both\n";
    std::vector<size_t> find;
    switch (check_input_int("option")){
    case 1:
        return FindByFilter(ps,checkByName,check_input_str("name of station('ctrl+z' to end input)"));
    case 2:
        return FindByFilter(ps,checkByEffcy,check_input_int("efficiency"));
    case 3:
        for     (auto temp_by_name:  FindByFilter(ps,checkByName,check_input_str("name of station('ctrl+z' to end input)"))){
            for (auto temp_by_state: FindByFilter(ps,checkByEffcy,check_input_int("efficiency"))){
                if (temp_by_name==temp_by_state)
                    find.emplace_back(temp_by_name);
            }
        }
        break;
    default:
        break;
    }
    return find;
}


bool ITC::selectPipes(std::map<size_t, pipe> &pipes){
    vector<size_t> find = filterSelectPipes(pipes);
    if (find.empty()) {
        cout << "Not found\n";
        return false;
    }
    else
        do{
        for (auto p: find) cout << pipes[p];
        if (check_ans("edit all?")){
            bool under_repair = check_ans("under repair?");
            for (auto p: find){
                pipes[p].under_repair=under_repair;
            }
        }
        else if (check_ans("edit one?"))
            if(!edit(pipes,check_input_int("ID"))) cout << "can't edit pipe\n";
    } while(check_ans("continue to edit?"));
    return true;
}

bool ITC::selectStations(std::map<size_t, station> &stations){
    vector<size_t> find = filterSelectStations(stations);
    if (find.empty()) {
        cout << "Not found\n";
        return false;
    }
    else
        do{
        for (auto s: find) cout << stations[s];
        if (check_ans("edit all?")){
            int quantity = check_input_int("quantity of departments");
            int quantity_in_work = check_input_int("quantity of departments in work");

            while(quantity < quantity_in_work) {
                cout<<"ERROR: to many departments in work, please reenter\n";
                quantity = check_input_int("quantity of departments");
                quantity_in_work = check_input_int("quantity of departments in work");
            }
            int efficiency = double(quantity_in_work)/quantity*100;

            for (auto s: find){
                stations[s].quantity=quantity;
                stations[s].quantity_in_work=quantity_in_work;
                stations[s].efficiency=efficiency;
            }
        }
        else if (check_ans("edit one?"))
            if(!edit(stations,check_input_int("ID"))) cout << "can't edit station\n";
    } while(check_ans("continue to edit?"));
    return true;
}
