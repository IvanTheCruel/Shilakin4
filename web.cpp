#include <web.h>
using namespace std;

web::web(){}

void web::edit(){
    size_t idr,idc;
    print();
    while(ITC::check_ans("\nwant to connect(disconnect) stations?")){
        idr = ITC::check_input_st_int("row");
        idc = ITC::check_input_st_int("column");
        if (key_map_id.find(idr) != key_map_id.end() && key_map_id.find(idc) != key_map_id.end()) {
            if (idr != idc) {
                int id = ITC::check_input_int("id of pipe to connect with(-1 to disconnect)");
                if (id == -1) detach(idr,idc);
                else          tie_in(idr,idc,id);
            } else cout<<"WARNING: station already connected to itself\n\n";
        } else cout<<"not found\n\n";
        print();
    }
    return;
}

bool web::detach(size_t r, size_t c){
    int pipe_id = adj_web[make_pair(r,c)];
    if (pipe_id != -1) {
        used_pipes[size_t(pipe_id)] = false;
        adj_web[make_pair(r,c)] = -1;
        return true;
    } else cout << "cannot detach anything\n";
    return false;
}

void web::tie_in(size_t idr, size_t idc, size_t id){
    if (used_pipes.find(id) != used_pipes.end()){
        if(adj_web[make_pair(idr,idc)]==-1){
            adj_web[make_pair(idr,idc)]=int(id);
            used_pipes[id] = true;
        } else cout<<"WARNING: already connected\n\n";
    } else cout<<"WARNING: pipe not found\n\n";
}

void web::add_pipe(bool edit){
    pipes.insert({ITC::pipe::get_max_id(),ITC::pipe(edit)});
    used_pipes.insert({ITC::pipe::get_max_id()-1, false});
}

void web::add_st(bool edit){
    size_t temp = ITC::station::get_max_id();
    stations.insert({temp,ITC::station(edit)});
    key_map_id.insert(temp);
    for (auto i: key_map_id){
        adj_web.emplace(make_pair(temp,i),-1);
        if (temp!=i) adj_web.emplace(make_pair(i,temp),-1);
    }
}

bool web::rebuild(){
    adj_web.clear();
    key_map_id.clear();
    used_pipes.clear();
    for (auto i: stations){
        key_map_id.insert(i.first);
        for (auto j: stations)
            adj_web.emplace(make_pair(i.first,j.first),-1);
    }
    for (auto i: pipes)
        used_pipes.insert(make_pair(i.first, false));
    return !stations.empty();
}

double web::weight(size_t id){
    if (pipes[id].under_repair) return numeric_limits<double>::infinity();
    return pipes[id].length;//pow(pipes[id].diameter, 2.5/pipes[id].length);
}

double web::max_throughput(size_t id){
    if (pipes[id].under_repair) return 0;
    return pipes[id].length;//pow(pipes[id].diameter, 2.5/pipes[id].length);
}

void web::print(){
    cout << "\t";
    for (auto i: key_map_id) {
        cout << " " << i << " \t";
    }
    for (auto r: key_map_id){
        cout << "\n\n" << r << "\t";
        for (auto c: key_map_id){
            int temp = adj_web[make_pair(r,c)];
            if (temp != -1)
                cout << temp << "\t";
            else
                cout << " - " << "\t";
        }
    }
    cout << "\nunused pipe id is: ";
    for (auto i: used_pipes){
        if(!i.second) cout << i.first << " ";
    }
    cout << "\n";
}

bool web::topological_sort(){
    sort.clear(); colours.clear(); visited.clear();
    vector<size_t> temp_key_map;
    for (auto c: key_map_id){
        int temp=0;
        colours.emplace(c,1);
        visited.emplace(c,0);
        for (auto r: key_map_id){
            //Проверить столбец вершины(как в неё можно попасть)
            temp += (adj_web[make_pair(r,c)]==-1? 0:1);
        }
        if (temp == 0) temp_key_map.push_back(c); //запомнить вершины корни
    }
    if(temp_key_map.size()!=0){
        for (auto v: temp_key_map) {
            dfs(v,key_map_id.size()+1);
        }
        cout << "\n";
        for (auto c: sort)
            cout << "id" << c.first << "_" << c.second << "\n";
        return true;
    } else
        cout<< "cycle\n";
    return false;
}

bool web::find_way(size_t to, size_t from){
    if(!(stations.find(from) != stations.end()) && (stations.find(to) != stations.end())){
        cout << "ID not found\n";
        return false;
    }
    map<int, pair<double,bool>> dkst;
    for (auto c: key_map_id)
        dkst.emplace(c,make_pair(numeric_limits<double>::infinity(),false));
    std::set<size_t> found;
    found.insert(from);
    dkst[int(from)].second=true;
    dkst[int(from)].first=0;
    if (!find_neighbor(dkst,found,to)){
        cout<<"\nCan't find path\n\n";
        return false;
    } else {
        cout<<"\nThe shortest path to " << to << " from " << from << " is " << dkst[int(to)].first << "\n\n";
        return true;
    }
}

bool web::find_neighbor(std::map<int, std::pair<double, bool> > &dkst, set<size_t> found, size_t to){
    map<int, pair<double,bool>> changes=dkst;
    set<size_t> check;
    for (auto f: found)     //для всех найденных ранее вершин
        for (auto c: key_map_id){ //посмотеть массив есть ли у него соседи
            if(adj_web[make_pair(f,c)]!=-1 && !dkst[int(c)].second) { //если есть ребро и оно не красное
                check.emplace(c); //надо посмотреть на следующем шаге
                dkst[int(c)].first = ITC::min(dkst[int(c)].first, dkst[int(f)].first+weight(adj_web[make_pair(f,c)]));//и проверить вес
            }
        }
    double min=numeric_limits<double>::infinity();
    int id=-1;
    size_t is_it_end = 0;
    for (auto s: dkst){
        if(!s.second.second){
            if (min > s.second.first) {
                min=s.second.first;
                id=s.first;
            }
        } else is_it_end++;
    }
    dkst[id].second=true;
    if (dkst[int(to)].second)   return true; //если нашли путь до искомой
    if (is_it_end==dkst.size()) return true; //если нашли всё
    if (dkst==changes)          return false;//если ничего не изменилось
    return find_neighbor(dkst,check,to);     //продолжить поиск
}

bool web::max_flow(size_t from, size_t to){
    if(!(stations.find(from) != stations.end()) && (stations.find(to) != stations.end())){
        cout << "ID not found\n";
        return false;
    }
    for (auto p: used_pipes) pipes[p.first].stream=0;

    return step_flow(to, from);
}


bool web::step_flow(size_t from, size_t to){
    map<size_t,mark> find;
    set<size_t> check;
    find.emplace(from, mark(true,from,numeric_limits<double>::infinity()));
    check.insert(from);

    bool may_be_problem_with_cycle = false;
    for (auto it = check.begin(); it != check.end();){ //для каждой вершины
        for (auto j: key_map_id){
            int pipe_id=adj_web[make_pair(*it,j)];
            if(pipe_id!=-1 && (find.find(j) == find.end())) //посмотреть нового соседа и чтоб у того не было метки
                if(max_throughput(size_t(pipe_id))!=pipes[size_t(pipe_id)].stream){ //максимальный поток по трубе и поток сейчас
                    may_be_problem_with_cycle=true;
                    check.emplace(j); //надо его посмотреть
                    find.emplace(j,mark(true,*it,ITC::min(find[*it].flow,max_throughput(size_t(pipe_id))-pipes[size_t(pipe_id)].stream)));
                }
            pipe_id=adj_web[make_pair(j,*it)];
            if(pipe_id!=-1 && (find.find(j) == find.end())) //посмотреть входящих соседей и чтоб у того не было метки
                if(pipes[size_t(pipe_id)].stream!=0){       //поток сейчас не нулевой
                    may_be_problem_with_cycle=true;
                    check.emplace(j); //надо его посмотреть
                    find.emplace(j,mark(false,*it,ITC::min(find[*it].flow,pipes[size_t(pipe_id)].stream)));
                }
        }
        //проверили вершину и убрали её из искомых
        auto temp_iter = it;
        it++; //двинули цикл
        check.erase(*temp_iter); //удалили
        if (may_be_problem_with_cycle) {
            it = check.begin();  //вернули цикл к наименьшей
            may_be_problem_with_cycle = false;
        }
        if (find.find(to)!=find.end()) break;
        if (check.empty()){
            double max_flow = 0;
            for (auto p: used_pipes)
                if (p.second && pipes[p.first].stream == max_throughput(p.first))
                    max_flow+=pipes[p.first].stream;
            if (max_flow==0) {
                cout << "KS can't deliver liquid to each other\n";
                return false;
            } else {
                cout << "\nmax troughput is " << max_flow << endl;
                return true;
            }
        }
    }
    //обратный поток
    size_t end = to;
    while(end!=from){
        size_t pipe_id = size_t(adj_web[make_pair(find[end].ID_prev,end)]);
        pipes[pipe_id].stream+=find[end].positive?find[end].flow:-find[end].flow;
        end=find[end].ID_prev;
    }

    return step_flow(from, to);
}


bool web::dfs(size_t v, size_t ts){
    ts--;
    colours[v] = 2; //мы тут были
    visited[v]++;
    int temp;
    for (auto c: key_map_id) { //куда можем попасть смотрим
        temp = adj_web[make_pair(v,c)];
        if (temp != -1){ //ребро должно существовать чтобы попасть
            if (colours[c]!=3 && visited[c]!=2){ //цвет вершины не конец
                                                               //и не цикл
                if (colours[c] == 1) {                  //если еще не были
                    if (dfs(c, ts)) return true;           //идем туда
                }
                else { if (colours[c] == 2) return true; } //если были забили
            }
        }
        if (colours[v] == 3) return false; //конечная
        if (visited[v] == 2) return false; //были два раза
    }
    colours[v] = 3; //конечная
    sort.emplace(v,ts);
    return false;
}

bool web::fout(string address){
    ofstream fout;
    fout.open(address);
    if (fout.is_open()){
        for (auto v: stations) fout << v.second;
        for (auto v: pipes)    fout << v.second;
        fout << *this;
    } else {
        cout<<"ERROR:file isn't open!\n\n";
        fout.close();
        return false;
    }
    fout.close();
    return true;
}

bool web::fin(string address){
    ifstream fin(address);
    if (fin.is_open()){
        char t;
        size_t tempid;
        pipes.clear(); stations.clear();
        ITC::pipe::kill_sId(); ITC::station::kill_sId();
        fin >> t;
        while(t != 'A' && t != 'e'){
            if (t=='S') {
                fin >> tempid;
                stations.insert({tempid,ITC::station(fin, tempid)});
            } else if (t=='P') {
                fin >> tempid;
                pipes.insert({tempid,ITC::pipe(fin, tempid)});
            }
            fin >> t;
        }
        rebuild();
        int id;
        for (auto r: key_map_id){
            for (auto c: key_map_id){
                fin >> id;
                if (id != -1) tie_in(r,c,size_t(id));
            }
            fin >> t;
        }
        fin.close();
    } else {
        cout<<"ERROR:file isn't open!\n\n";
        fin.close();
        return false;
    }
    return true;
}

void web::clean(){
    for (auto v: adj_web) if(v.second!=-1) detach(v.first.first,v.first.second);
}

void web::delete_st(){
    size_t id = ITC::check_input_st_int("ID");
    if (stations.find(id) != stations.end()) {
        stations.erase(id);
        for (auto i: key_map_id) {
            detach(i,id);
            adj_web.erase(make_pair(i,id));
            if (id!=i){
                detach(id, i);
                adj_web.erase(make_pair(id,i));
            }
        }
        key_map_id.erase(id);
    } else cout << "ID not found\n";
}

void web::delete_pipe(){
    size_t id = ITC::check_input_st_int("ID");
    if (pipes.find(id) != pipes.end()) {
        pipes.erase(id);
        if (used_pipes[id])
            for (auto i: adj_web)
                if (int(id) == i.second){
                    i.second = -1;
                    break;
                }
        used_pipes.erase(id);
    } else cout << "ID not found\n";
}

ofstream &operator<<(ofstream & ofs, const web &web){
    string ans;
    size_t k=0;
    for (auto i: web.adj_web){
        ans+=(k%web.key_map_id.size()==0?"A ":"")+to_string(i.second)+" "+((k+1)%web.key_map_id.size()==0?"\n":"");
        k++;
    }
    ans+="end";
    ofs << ans;
    return ofs;
}
