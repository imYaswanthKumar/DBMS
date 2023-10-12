#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

const ll MOD=1e9+9;
class Buckets{

    private:

    ll level, bucketsize, currentsize;
    ll *arr;

    public:

    Buckets(ll level,ll bucketsize){
        this->level=level;
        this->bucketsize=bucketsize;
        this->currentsize=0;
        arr=new ll[bucketsize];
    }

    bool possible(){
        return (this->currentsize < bucketsize);
    }

    void insertNode(ll value){
        this->arr[currentsize]=value;
        this->currentsize++;
        return;
    }

    ll get_element(ll i){
        return this->arr[i];
    }

    ll getLevel(){
        return this->level;
    }

    ll getBucketSize(){
        return this->bucketsize;
    }

    ll getCurrentSize(){
        return this->currentsize;
    }

    void set_element(ll i,ll val){
        this->arr[i]=val;
        this->currentsize=i+1;
    }

    void decreaseLevel(){
        this->level--;
        return;
    }

    void eraseElement(ll val){
        // this->arr
        ll *newarr = new ll[this->bucketsize];
        ll ptr=0;
        for(ll i=0;i<this->currentsize;i++){
            if(arr[i]!=val){
                newarr[ptr]=arr[i];
                ptr++;
            }
        }
        delete[] arr;
        arr=newarr;
        this->currentsize=ptr;
        return;
    }
    ~Buckets(){
        delete [] arr;
    }
};


class Directory{
    private:

    ll level=1;
    ll bucketsize;
    vector<Buckets*> directories;

    public:

    Directory(ll bucketsize){
        this->level=1;
        this->bucketsize=bucketsize;
        directories.resize(2);
        for(ll i=0; i<(1<<level); i++){
            directories[i]=new Buckets(1,bucketsize);
        }
    }

    ll getLevel(){
        return this->level;
    }

    Buckets* getDir(ll ref){
        return this->directories[ref];
    }

    void setDir(ll i,Buckets* buck){
        this->directories[i]=buck;
    }
    // insertion
    ll increaseSize(ll prev,ll value_to_be_inserted){

        ll addit=(1<<level);

        for(ll i=0;i<(1<<level);i++){
            this->directories.push_back(this->directories[i]);
        }

        level++;

        Buckets* one=new Buckets(level,bucketsize);
        Buckets* two=new Buckets(level,bucketsize);
        Buckets* three=new Buckets(level,bucketsize);

        ll modulo=(1<<level);

        ll it1=0,it2=0;

        for(ll i=0;i<bucketsize;i++){
            ll element=this->directories[prev]->get_element(i);
            ll ref=element%modulo;
            if(ref==prev){
                if(it1!=bucketsize){
                    one->set_element(it1,element);
                    it1++;
                }else{
                    this->directories[addit+prev]=three;
                    this->directories[prev]=one;
                    // delete one;
                    delete two;
                    // delete three;
                    return prev;
                }
            }else{
                if(it2!=bucketsize){
                    two->set_element(it2,element);
                    it2++;
                }else{
                    this->directories[prev]=three;
                    this->directories[addit+prev]=two;
                    delete one;
                    // delete two;
                    // delete three;
                    return addit+prev;
                }
            }
        }

        ll ref=value_to_be_inserted % modulo;
        if(ref==prev){
            if(it1!=bucketsize){
                one->set_element(it1,value_to_be_inserted);
                it1++;
            }else{
                this->directories[addit+prev]=three;
                this->directories[prev]=one;
                // delete one;
                delete two;
                // delete three;
                return prev;
            }
        }else{
            if(it2!=bucketsize){
                two->set_element(it2,value_to_be_inserted);
                it2++;
            }else{
                this->directories[prev]=three;
                this->directories[addit+prev]=two;
                delete one;
                // delete two;
                // delete three;
                return addit+prev;
            }
        }

        this->directories[prev]=one;
        this->directories[addit+prev]=two;
        // delete one;
        // delete two;
        delete three;
        return -1;

    }

    ll split(ll prev,ll addit,ll value_to_be_inserted,ll new_level){
        ll it1=0,it2=0;
        ll modulo=addit;
        Buckets* one=new Buckets(new_level,bucketsize);
        Buckets* two=new Buckets(new_level,bucketsize);
        Buckets* three=new Buckets(new_level,bucketsize);
        ll next=prev+(addit/2);
        for(ll i=0;i<bucketsize;i++){
            ll element=this->directories[prev]->get_element(i);
            ll ref=element%modulo;
            if(ref==prev){
                if(it1!=bucketsize){
                    one->set_element(it1,element);
                    it1++;
                }else{
                    for(ll j=next;j<(1<<this->level);j+=addit){
                        this->directories[j]=three;
                    }
                    for(ll j=prev;j<(1<<this->level);j+=addit){
                        this->directories[j]=one;
                    }
                    // delete one;
                    delete two;
                    // delete three;
                    return prev;
                }
            }else{
                if(it2!=bucketsize){
                    two->set_element(it2,element);
                    it2++;
                }else{
                    for(ll j=prev;j<(1<<this->level);j+=addit){
                        this->directories[j]=three;
                    }
                    for(ll j=next;j<(1<<this->level);j+=addit){
                        this->directories[j]=two;
                    }
                    delete one;
                    // delete two;
                    // delete three;
                    return next;
                }
            }
        }

        ll ref=value_to_be_inserted % modulo;
        if(ref==prev){
            if(it1!=bucketsize){
                one->set_element(it1,value_to_be_inserted);
                it1++;
            }else{
                for(ll j=next;j<(1<<this->level);j+=addit){
                    this->directories[j]=three;
                }
                for(ll j=prev;j<(1<<this->level);j+=addit){
                    this->directories[j]=one;
                }
                // delete one;
                delete two;
                // delete three;
                return prev;
            }
        }else{
            if(it2!=bucketsize){
                two->set_element(it2,value_to_be_inserted);
                it2++;
            }else{
                for(ll j=prev;j<(1<<this->level);j+=addit){
                    this->directories[j]=three;
                }
                for(ll j=next;j<(1<<this->level);j+=addit){
                    this->directories[j]=two;
                }
                delete one;
                // delete two;
                // delete three;
                return next;
            }
        }
        for(ll j=prev;j<(1<<this->level);j+=addit){
            this->directories[j]=one;
        }
        for(ll j=next;j<(1<<this->level);j+=addit){
            this->directories[j]=two;
        }
        // delete one;
        // delete two;
        delete three;
        return -1;
    }

    // deletion
    bool merging(ll ref,ll conj){
        ll sj1=this->directories[ref]->getCurrentSize();
        ll sj2=this->directories[conj]->getCurrentSize();
        ll ptr1=sj1;
        for(ll i=0;i<sj2;i++){
            ll element=this->directories[conj]->get_element(i);
            this->directories[ref]->set_element(ptr1,element);
        }
        this->directories[ref]->decreaseLevel();
        // this->directories[conj]=this->directories[ref];
        // ll level2=this->directories[conj]->getLevel();
        Buckets* buck=this->directories[conj];
        ll maxi=1;
        for(ll i=0;i<(1<<this->level);i++){
            if(this->directories[i]==buck){
                this->directories[i]=this->directories[ref];
            }
            maxi=max(maxi,this->directories[i]->getLevel());
        }

        if(maxi<this->level){
            // shrinking possible
            this->level--;
            this->directories.resize(1<<this->level);
            return true;
        }
        return false;

    }

    void deleteElement(ll ref,ll value){
        bool flag=false;
        for(ll i=0;i<this->bucketsize;i++){
            ll element=this->directories[ref]->get_element(i);
            if(element==value){
                flag=true;
                break;
            }
        }
        if(!flag){
            cout<<"Element "<<value<<" was not inserted previously.\n";
            return;
        }
        this->directories[ref]->eraseElement(value);

        while(true){
            if(this->level==1){
                break;
            }
            ll ufu=(1<<this->level);
            ufu/=2;
            ll conj=ref;
            if(ref<ufu){
                conj+=ufu;
            }else{
                conj-=ufu;
            }
            ll sj1=this->directories[ref]->getCurrentSize();
            ll sj2=this->directories[conj]->getCurrentSize();

            if(sj1+sj2>this->bucketsize){
                break;
            }
            if(ref>conj){
                swap(ref,conj);
            }
            bool flag=merging(ref,conj);
            if(!flag){
                break;
            }
        }
    }

    void display(){
        cout<<"Directory level is: "<<this->level<<"\n";

        for(ll ind=0;ind<(1<<level);ind++){
            cout<<"Index is "<<ind<<" , level is "<<this->directories[ind]->getLevel()<<" , no.of elements are: "<<this->directories[ind]->getCurrentSize()<<"\n";
            ll sj=this->directories[ind]->getCurrentSize();
            for(ll i=0;i<sj;i++){
                ll element=this->directories[ind]->get_element(i);
                cout<<element<<" \n"[i==sj-1];
            }
        }

        cout<<"Done\n";
    }
};

void deletion(Directory& dir){
    ll value;
    cout<<"Enter the value to be deleted\n";
    cin>>value;

    ll level=dir.getLevel();
    ll modulo=(1ll<<level);
    ll ref=value%modulo;

    dir.deleteElement(ref,value);

}
void insertion(Directory& dir){
    ll value;
    cout<<"Enter the value to be inserted:\n";
    cin>>value;

    ll level=dir.getLevel();
    ll modulo=(1<<level);
    ll ref=value % modulo;
    Buckets* buck=dir.getDir(ref);
    ll level2=buck->getLevel();
    ll bucketsize=buck->getBucketSize();
    for(ll i=0;i<bucketsize;i++){
        ll element=buck->get_element(i);
        if(element==value){
            cout<<value<<" was already inserted previously.\n";
            return;
        }
    }
    if(buck->possible()){
        buck->insertNode(value);
        return;
    }else if(buck->getLevel() < dir.getLevel()){
        while(true){
            ll localdepth=level2;
            ref=value%(1<<localdepth);
            ll addit=(1<<(level2+1));
            ll ok=dir.split(ref,addit,value,level2+1);
            if(ok==-1){
                break;
            }else{
                ref=ok;
            }
            level2++;
        }
        return;
    }

    while(true){
        ll ok=dir.increaseSize(ref,value);
        if(ok==-1){
            break;
        }else{
            ref=ok;
        }
    }

    return;

}

void solve(){
    // sample testcase in comments
    ll bucketsize;
    cout<<"Enter the size of bucket:\n";
    cin>>bucketsize;
    // 2
    Directory* dir=new Directory(bucketsize);
    ll count1;
    cout<<"Enter the no.of elements you want to insert:\n";
    cin>>count1;

    // 32 64 128
    for(ll it=0;it<count1;it++){
        insertion(*dir);
        dir->display();
    }

    ll count2;
    cout<<"Enter the no.of elements you want to delete:\n";
    cin>>count2;
    // 64 to be removed
    for(ll it=0;it<count2;it++){
        deletion(*dir);
        dir->display();
    }

}

int main()
{
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);cout.tie(NULL);
    ll t=1;
    // cin>>t;
    while(t--){
        solve();
    }
    return 0;
}

// some if conditions are useless, find them out.
