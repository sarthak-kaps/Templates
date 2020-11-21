class DSU {
    typedef long long int lli;
private :
    vector<lli> set;
public :
    DSU(int n){
        set.assign(n, -1);
    }
    int size(int i){
        i = parent(i);
        return -set[i];
    }
    int parent(int i){    // find parent and path compression
        if(set[i] <= -1){
            return i;
        }
        set[i] = parent(set[i]);
        return set[i];
    }
    void Union(int i, int j){
        i = parent(i);
        j = parent(j);
        if(i == j){
            return;
        }
        set[j] = i;
    }
    lli getDistinct(){
        lli ct = 0;
        for(int i = 0; i < set.size(); i++){
            if(set[i] < 0) ct++;
        }
        return ct;
    }
};
