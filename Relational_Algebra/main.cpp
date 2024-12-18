#include<bits/stdc++.h>
using namespace std;

vector<string> operator + (vector<string>& a, vector<string>& b){
    auto v = a;
    for(auto s : b) v.push_back(s);
    return v;
}

void toupper(string& s){
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    for(auto &c : s) c = toupper(c);
}

bool is_integer(string& s){
    return all_of(s.begin() + int(s[0] == '-'), s.end(), ::isdigit);
}

map<string, int> precedence{ {"|", 0}, {"^", 1}, {"!", 2}, {"=", 2}, {">", 3}, {"<", 3}, {"+", 4}, {"-", 4}, {"*", 5}, {"/", 5}, {"%", 5}};
bool success = true;
vector<string> table_list = {"STUDENT", "COURSE", "ENROLLMENT", "PROFESSOR", "DEPARTMENT"};

class Relation{
public:
    string name;
    set<vector<string>> records;
    vector<string> att_list;
    map<string, int> att_map;
};

string remove_brackets(string& s){
    int i = 0, j = s.size() - 1;
    while(s[i] == '(' && s[j] == ')') i++, j--;
    string t;
    for(int k = i; k <= j; k++) t.push_back(s[k]);
    return t;
}

vector<string> split(string s, char delim){
    vector<string> res;
    stringstream ss(s);
    string temp;
    while(getline(ss, temp, delim)) res.push_back(temp);
    return res;
}

vector<string> postfix(string query){
    stack<string> s;
    vector<string> pf;
    for(int i = 0; i < query.size(); i++){
        if(query[i] == ' ') continue;
        if(query[i] == '('){
            s.push("(");
            continue;
        }
        if(query[i] == ')'){
            while(!s.empty() && s.top() != "(") pf.push_back(s.top()), s.pop();
            s.pop();
            continue;
        }
        if(!precedence.count(string(1, query[i]))){
            pf.push_back(string(1, query[i++]));
            while(i < query.size() && !precedence.count(string(1, query[i])) && query[i] != '(' && query[i] != ')'){
                if(query[i] != ' ') pf.back().push_back(query[i]);
                i++;
            }
            i--;
            continue;
        }
        string op = string(1, query[i]);
        while(!s.empty() && s.top() != "(" && precedence[s.top()] >= precedence[op]) pf.push_back(s.top()), s.pop();
        if(query[i + 1] == '=') op.push_back('='), i++;
        s.push(op);
    }
    while(!s.empty()) pf.push_back(s.top()), s.pop();
    return pf;
}

Relation project(string query, Relation table){
    if(query == "*") return table;
    Relation out;
    if(!success) return out;
    out.name = table.name;
    out.att_list = split(query, ',');
    int att_no = 0;
    for(auto att : out.att_list){
        if(!table.att_map.count(att)){
            cout << "ERROR : Column " << att << " does not exist\n";
            success = false;
            return out;
        }
        out.att_map[att] = att_no++;
    }
    for(auto x : table.records){
        vector<string> temp;
        for(auto y : out.att_list)
            temp.push_back(x[table.att_map[y]]);
        out.records.insert(temp);
    }
    return out;
}

Relation select(string query, Relation table){
    if(!success) return *new Relation;
    auto p = postfix(query);
    stack<pair<Relation, vector<string>>> s;
    for(auto i : p){
        if(!precedence.count(string(1, i[0]))){
            Relation r = table;
            vector<string> v;
            if(i[0] == '\'' && i.back() == '\''){
                i.erase(i.begin());
                i.pop_back();
                for(auto it : r.records) v.push_back(i);
            }
            else{
                if(!r.att_map.count(i)){
                    cout << "ERROR : Column " << i << " does not exist\n";
                    success = false;
                    return r;
                }
                int att_no = r.att_map[i];
                for(auto it : r.records) v.push_back(it[att_no]);
            }
            s.emplace(r, v);
            continue;
        }
        if(precedence[string(1, i[0])] >= 4){
            auto v2 = s.top().second; s.pop();
            auto [r, v] = s.top(); s.pop();
            for(int it = 0; it < v.size(); it++){
                if(!is_integer(v[it]) || !is_integer(v2[it])){
                    cout << "ERROR : Incorrect data type, cannot perform arithmetic operation\n";
                    success = false;
                    return r;
                }
            }
            if(i == "+")
                for(int it = 0; it < v.size(); it++)
                    v[it] = to_string(stoll(v[it]) + stoll(v2[it]));
            else if(i == "-")
                for(int it = 0; it < v.size(); it++)
                    v[it] = to_string(stoll(v[it]) - stoll(v2[it]));
            else if(i == "*")
                for(int it = 0; it < v.size(); it++)
                    v[it] = to_string(stoll(v[it]) * stoll(v2[it]));
            else if(i == "/")
                for(int it = 0; it < v.size(); it++)
                    v[it] = to_string(stoll(v[it]) / stoll(v2[it]));
            else if(i == "%")
                for(int it = 0; it < v.size(); it++)
                    v[it] = to_string(stoll(v[it]) % stoll(v2[it]));
            s.emplace(r, v);
            continue;
        }
        if(precedence[string(1, i[0])] >= 2){
            auto v2 = s.top().second; s.pop();
            auto [r1, v1] = s.top(); s.pop();
            Relation r = r1;
            r.records.clear();
            int j = 0;
            bool is_int = true;
            for(int it = 0; it < v1.size(); it++){
                if(!is_integer(v1[it]) || !is_integer(v2[it])){
                    is_int = false; break;
                }
            }
            if(i == "!="){
                for(auto it : r1.records){
                    if(v1[j] != v2[j]) r.records.insert(it);
                    j++;
                }
            }
            else if(i == "="){
                for(auto it : r1.records){
                    if(v1[j] == v2[j]) r.records.insert(it);
                    j++;
                }
            }
            else if(i == ">"){
                for(auto it : r1.records){
                    if(is_int){
                        if(stoll(v1[j]) > stoll(v2[j])) r.records.insert(it);
                    }
                    else if(v1[j] > v2[j]) r.records.insert(it);
                    j++;
                }
            }
            else if(i == ">="){
                for(auto it : r1.records){
                    if(is_int){
                        if(stoll(v1[j]) >= stoll(v2[j])) r.records.insert(it);
                    }
                    else if(v1[j] >= v2[j]) r.records.insert(it);
                    j++;
                }
            }
            else if(i == "<"){
                for(auto it : r1.records){
                    if(is_int){
                        if(stoll(v1[j]) < stoll(v2[j])) r.records.insert(it);
                    }
                    else if(v1[j] < v2[j]) r.records.insert(it);
                    j++;
                }
            }
            else if(i == "<="){
                for(auto it : r1.records){
                    if(is_int){
                        if(stoll(v1[j]) <= stoll(v2[j])) r.records.insert(it);
                    }
                    else if(v1[j] <= v2[j]) r.records.insert(it);
                    j++;
                }
            }
            s.emplace(r, v1);
            continue;
        }
        auto r2 = s.top().first; s.pop();
        auto [r1, v] = s.top(); s.pop();
        if(i == "|") r1.records.insert(r2.records.begin(), r2.records.end());
        else
            for(auto it : r1.records)
                if(!r2.records.count(it)) r1.records.erase(it);
        s.emplace(r1, v);
    }
    return s.top().first;
}

Relation rename(string query, Relation table){
    if(!success) return table;
    Relation out;
    if((query.find("(") == string::npos && query.find(")") != string::npos) || (query.find("(") != string::npos && query.find(")") == string::npos)){
        cout << "ERROR : Invalid query\n";
        success = false;
        return out;
    }
    if(query.find("(") == string::npos && query.find(")") == string::npos){
        out = table;
        out.name = query;
        return out;
    }
    int st = query.find("("), en = query.find(")");
    out.name = st ? query.substr(0, st) : table.name;
    query = query.substr(st + 1, en - st - 1);
    out.att_list = split(query, ',');
    if(out.att_list.size() != table.att_list.size()){
        cout << "ERROR : Attribute count does not match\n";
        success = false;
        return out;
    }
    out.records = table.records;
    int att_no = 0;
    for(auto att : out.att_list) out.att_map[att] = att_no++;
    return out;
}

Relation Union(Relation t1, Relation t2){
    Relation out;
    if(!success) return out;
    if(t1.att_list != t2.att_list){
        cout << "ERROR : Tables are not union compatible\n";
        success = false;
        return out;
    }
    out = t1;
    out.name = t1.name + " \u222A " + t2.name;
    out.records.insert(t2.records.begin(), t2.records.end());
    return out;
}

Relation set_diff(Relation t1, Relation t2){
    Relation out;
    if(!success) return out;
    if(t1.att_list != t2.att_list){
        cout << "ERROR : Tables are not union compatible\n";
        success = false;
        return out;
    }
    out = t1;
    out.name = t1.name + " \u2216 " + t2.name;
    for(auto x : t1.records)
        if(t2.records.count(x)) out.records.erase(x);
    return out;
}

Relation intersect(Relation t1, Relation t2){
    if(!success) return t1;
    Relation out = set_diff(set_diff(Union(t1, t2), set_diff(t1, t2)), set_diff(t2, t1));
    out.name = t1.name + " \u2229 " + t2.name;
    return out;
}

Relation cartesian(Relation t1, Relation t2){
    Relation out;
    if(!success) return out;
    if(t1.name == t2.name){
        cout << "ERROR : The tables must be different\n";
        success = false;
        return out;
    }
    out.name = t1.name + " \u2A2F " + t2.name;
    set<string> s1, s2;
    for(auto atts : t2.att_list) s2.insert(atts);
    for(auto atts : t1.att_list){
        s1.insert(atts);
        out.att_list.push_back(s2.count(atts) ? t1.name + "." + atts : atts);
    }
    for(auto atts : t2.att_list) out.att_list.push_back(s1.count(atts) ? t2.name + "." + atts : atts);
    int att_no = 0;
    for(auto att : out.att_list) out.att_map[att] = att_no++;
    for(auto x : t1.records)
        for(auto y : t2.records) out.records.insert(x + y);
    return out;
}

Relation process(string query);

class Table{
    Relation table;
public:
    Table(string name){
        table.name = name;
        read(name + ".csv");
    }

    void read(string file_name){
        ifstream f(file_name);
        if(!f.good()){
            success = false;
            return;
        }
        string line;
        getline(f, line);
        table.att_list = split(line, ',');
        for(int i = 0; i < table.att_list.size(); i++) table.att_map[table.att_list[i]] = i;
        while(getline(f, line)) table.records.insert(split(line, ','));
    }

    Relation parse(string query){
        stack<pair<char, string>> s;
        while(query != table.name){
            int st = query.find("[");
            int k = 1, en = st;
            while(k){
                en++;
                if(query[en] == '[') k++;
                else if(query[en] == ']') k--;
            }
            string arg = query.substr(st + 1, en - st - 1);
            s.emplace(query[0], arg);
            query = query.substr(en + 1, query.size() - en - 1);
            query = remove_brackets(query);
        }
        Relation out = table;
        while(!s.empty()){
            auto [op, arg] = s.top();
            s.pop();
            switch(op){
                case 'P':
                    out = project(arg, out);
                    break;
                case 'S':
                    out = select(arg, out);
                    break;
                case 'R':
                    out = rename(arg, out);
                    break;
                case 'U':
                    out = Union(process(arg), out);
                    break;
                case 'I':
                    out = intersect(process(arg), out);
                    break;
                case 'D':
                    out = set_diff(process(arg), out);
                    break;
                case 'C':
                    out = cartesian(process(arg), out);
                    break;
                default:
                    cout << "ERROR : Invalid query\n";
                    success = false;
                    break;
            }
        }
        return out;
    }
};

Relation process(string query){
    string temp = query;
    bool flag = false;
    int k = 0;
    string str;
    for(auto c : temp){
        if(k < 0){flag = true; break;}
        if(c == '[') k++;
        else if(c == ']') k--;
        else if(k == 0) str.push_back(c);
    }
    if(k || str.empty()) flag = true;
    temp = str;
    int st = -1;
    for(int i = 0; i < temp.size() && !flag; i++){
        if(k < 0 || flag){
            flag = true;
            break;
        }
        if(temp[i] == '(') st = i, k++;
        else if(temp[i] == ')') k--;
    }
    if(k) flag = true;
    if(st != -1){
        str.clear();
        for(int i = st + 1; i < temp.size() && !flag; i++){
            if(temp[i] == ')') break;
            str.push_back(temp[i]);
        }
    }
    if(str.empty()) flag = true;
    Relation out;
    if(flag){
        cout<<"ERROR: Invalid query\n";
        success = false;
    }
    if(find(table_list.begin(), table_list.end(), str) == table_list.end()){
        cout << "ERROR: Table " << str << " does not exist\n";
        success = false;
    }
    else{
        Table T(str);
        out = T.parse(remove_brackets(query));
    }
    return out;
}

ostream& operator << (ostream& os, Relation& out){
    cout << '\n' << out.name << '\n';
    vector <int> sz(out.att_list.size(), 0);
    int w = 0;
    for(int i = 0; i < out.att_list.size(); i++){
    	sz[i] = out.att_list[i].size();
    	for(auto it : out.records)
    		sz[i] = max(sz[i], (int)it[i].size());
    	w += sz[i];
    }
    int width = w - 2 + 5 * out.att_list.size();
    for(int i = 0; i < width; i++) cout << "\u2013"; cout << '\n';
    for(int i = 0; i < out.att_list.size(); i++) cout << left << setw(sz[i]) << out.att_list[i] << "  \u007C  ";
    cout << '\n';
    for(int i = 0; i < width; i++) cout << "\u2013"; cout << '\n';
    for(auto it : out.records){
        for(int i = 0; i < it.size(); i++) cout << left << setw(sz[i]) << it[i] <<"  \u007C  ";
        cout << '\n';
    }
    for(int i = 0; i < width; i++) cout << "\u2013"; cout << '\n';
    return os;
}

int main(){
    ios::sync_with_stdio(false);

    string query;
    while(true){
        cout << ">> ";
        getline(cin, query);
        toupper(query);
        if(query == "EXIT") break;
        if(query.empty()) continue;
        success = true;
        Relation out = process(query);
        if(success) cout << out << '\n';
    }
    return 0;
}