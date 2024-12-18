#include<bits/stdc++.h>
using namespace std;

template <typename T>
void operator += (vector<T>& a, vector<T>& b){
    for(auto x : b) a.push_back(x);
    b.clear();
}

int order, order_leaf;
class Node{
public:
    vector<int> key;
    vector<Node*> children;
    Node* next;

    Node* insert(int val){
        if(children.empty()){
            auto it = lower_bound(key.begin(), key.end(), val);
            if(it != key.end() && *it == val) return nullptr;
            key.insert(it, val);
            if(key.size() <= order_leaf) return nullptr;
            int j = order_leaf / 2;
            next = new Node();
            for(int i = j + 1; i < key.size(); i++) next -> key.push_back(key[i]);
            key.resize(j + 1);
            return next;
        }
        auto child = children[lower_bound(key.begin(), key.end(), val) - key.begin()];
        auto new_child = child -> insert(val);
        if(new_child){
            int x = child -> key.back();
            auto it = lower_bound(key.begin(), key.end(), x);
            if(it == key.end() || *it != x) key.insert(it, x);
            if(!child -> children.empty()) child -> key.pop_back();
            x = new_child -> key.back();
            int i = lower_bound(key.begin(), key.end(), x) - key.begin();
            children.insert(children.begin() + i, new_child);
        }
        if(key.size() >= order){
            int j = (order - 1) / 2;
            auto sib = new Node();
            sib -> children.push_back(children[j + 1]);
            for(int i = j + 1; i < key.size(); i++) sib -> key.push_back(key[i]), sib -> children.push_back(children[i + 1]);
            key.resize(j + 1);
            children.resize(j + 1);
            return sib;
        }
        return nullptr;
    }

    void Delete(int val){
        if(children.empty()){
            auto it = find(key.begin(), key.end(), val);
            if(it != key.end()) key.erase(it);
            return;
        }
        int id = lower_bound(key.begin(), key.end(), val) - key.begin();
        int left = id - 1, right = id + 1;
        children[id] -> Delete(val);
        if(children[id] -> children.empty()){
            if(children[id] -> key.size() >= (order_leaf + 1) / 2) return;
            if(id > 0 && children[left] -> key.size() > (order_leaf + 1) / 2){
                children[id] -> key.insert(children[id] -> key.begin(), children[left] -> key.back());
                children[left] -> key.pop_back();
                key[left] = children[left] -> key.back();
            }
            else if(id < key.size() && children[right] -> key.size() > (order_leaf + 1) / 2){
                children[id] -> key.push_back(children[right] -> key[0]);
                children[right] -> key.erase(children[right] -> key.begin());
                key[id] = children[id] -> key.back();
            }
            else{
                if(id > 0){
                    children[left] -> key += children[id] -> key;
                    children[left] -> next = children[id] -> next;
                    children[id] -> next = nullptr;
                    children.erase(children.begin() + id);
                    key.erase(key.begin() + left);
                }
                else{
                    children[id] -> key += children[right] -> key;
                    children[id] -> next = children[right] -> next;
                    children[right] -> next = nullptr;
                    children.erase(children.begin() + right);
                    key.erase(key.begin() + id);
                }
            }
            return;
        }
        if(children[id] -> children.size() >= (order + 1) / 2) return;
        if(id > 0 && children[left] -> children.size() > (order + 1) / 2){
            children[id] -> key.insert(children[id] -> key.begin(), key[left]);
            children[id] -> children.insert(children[id] -> children.begin(), children[left] -> children.back());
            key[left] = children[left] -> key.back();
            children[left] -> key.pop_back();
            children[left] -> children.pop_back();
        }
        else if(id < key.size() && children[right] -> children.size() > (order + 1) / 2){
            children[id] -> key.push_back(key[id]);
            children[id] -> children.push_back(children[right]);
            key[id] = children[right] -> key[0];
            children[right] -> key.erase(children[right] -> key.begin());
            children[right] -> children.erase(children[right] -> children.begin());
        }
        else{
            if(id > 0){
                children[left] -> key.push_back(key[left]);
                children[left] -> key += children[id] -> key;
                children[left] -> children += children[id] -> children;
                children.erase(children.begin() + id);
                key.erase(key.begin() + left);
            }
            else{
                children[id] -> key.push_back(key[id]);
                children[id] -> key += children[right] -> key;
                children[id] -> children += children[right] -> children;
                children.erase(children.begin() + right);
                key.erase(key.begin() + id);
            }
        }
    }
};

ostream& operator << (ostream& os, Node* root){
    queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        int x = q.size();
        while(x--){
            auto node = q.front();
            q.pop();
            for(auto c : node -> children) q.push(c);
            for(auto k : node -> key) cout << k << (k == node -> key.back() ? " " : " \u2013 ");
            if(x) cout << "  \u007C  ";
        }
        cout << '\n';
    }
    return os;
}

int main(){
    ios::sync_with_stdio(false);

    cout << "Order of an internal node : ";
    cin >> order;
    cout << "Order of a leaf node : ";
    cin >> order_leaf;
    cout << "\nUsage :\n+key for inserting a key\n-key for deleting a key\n* for displaying the tree\n\n";
    Node* root = new Node();
    char c;
    int key;
    while(true){
        cout << ">> ";
        cin >> c;
        switch(c){
            case '+':{
                cin >> key;
                auto node = root -> insert(key);
                if(node){
                    auto new_root = new Node();
                    new_root -> children.push_back(root);
                    new_root -> children.push_back(node);
                    new_root -> key.push_back(root -> key.back());
                    if(!root -> children.empty()) root -> key.pop_back();
                    root = new_root;
                }
                break;
            }
            case '-':{
                cin >> key;
                root -> Delete(key);
                if(root -> children.size() == 1){
                    auto temp = root;
                    root = root -> children[0];
                    temp -> children.clear();
                }
                break;
            }
            case '*':{
                cout << root;
                break;
            }
            default:
                exit(0);
        }
    }
}
