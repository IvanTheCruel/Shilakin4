#include <check_input.h>
using namespace std;

string ITC::check_input(vector<string> a, string name_element){
    cout << name_element << "(";
    for (size_t i = 0; i < a.size(); i++) {
        cout << a[i];
        if(i+1 < a.size()) cout << "/";
    }
    cout << ")\n";
    //перечисляем подходящие значения

    string temp;                      //ввод пользователя
    bool idiot = true;                //по умолчанию пользователь идиот
    cin >> temp;

    for (size_t i = 0; i < a.size() && idiot; i++){
        if (temp == a[i]) idiot = false; //если ответ пользователя совпадает с желаемым, он не идиот
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(idiot){
        cout << "ERROR: wrong value!\n";
        return ITC::check_input(a, name_element); //если пользователь все же идиот, просим его ввести еще раз
    } else {
        return temp; //верни значение пользователя
    }
}

double ITC::check_input(string name_element){
    double input;
    bool valid= false;
    do
    {
        cout << "Enter a number of " << name_element << endl;
        cin >> input;
        if (cin.good()){
            valid = true;
        }
        else{
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "ERROR: wrong value! Please re-enter." << endl;
        }
    } while (!valid);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return input;

}

int ITC::check_input_int(string name_element){    
    double input;
    cout << "Enter a number of " << name_element << endl;
    cin >> input;

    while (cin.fail() || (input-int(input))){
        cout << "ERROR: nubmer of elements has to be integer. Try again: " << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (int)input;
}

string ITC::check_input_str(string name_element){
    std::string input="";
    std::string t;
    cout << "Enter " << name_element << endl;

    while(input.size() == 0){
        while(cin >> t || cin.get()=='\n'){
            if(!input.empty())input=input+" "+t;
            else input=t;
        }
    }

    for(size_t i = 0; i < input.size(); i++){
        if(input[i]=='|' || input[i]=='\n'){
            input.erase(input.begin()+i);
        }
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return input;
}

bool ITC::check_ans(std::string q){
    return "y"==ITC::check_input({"y","n"}, q);
}



size_t ITC::check_input_st_int(string name_element){
    size_t input;
    cout << "Enter a number of " << name_element << endl;
    cin >> input;

    while (cin.fail() || (input-size_t(input))){
        cout << "ERROR: nubmer of elements has to be integer. Try again: " << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (size_t)input;
}
