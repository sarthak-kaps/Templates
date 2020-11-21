typedef long long int lli;

class segTreeNode {
public :
    /* Class Data Members */

    /* This node is a class implementing the following methods: 
    * constructor: identity element of monoid (you should have the neutral values)
    * constructor: element created by combining 2 elements
    */
    segTreeNode() {}
    segTreeNode(const segTreeNode& one, const segTreeNode& two) {}  
};

class Function {
public :
    /* Function Data Members */

    /* A Function is a class implementing the following methods: 
    * constructor: identity function
    * constructor : to construct arbitrary function
    * constructor: function composition (f1.f2)
    * whether this function is the identity function
    * function definition (action on segTreeNode)
    */
    Function() {}  
    Function(/**/) {}
    Function(const Function& f1, const Function& f2) {}  
    bool is_identity() const {}  
    segTreeNode operator()(const segTreeNode& node) {}  

};

template<class M, class F>
class SegTree {
// M is a node
// F is a function
// remember to set leaf nodes from calling function
public:
    typedef M value_type;
    typedef F func_type;

    static void identity_check() {
        if(!F().is_identity()) {
            cout << "Default Constructor is not identity !!!" << endl;
        }
    }

private:
    lli n;
    vector<M> values;  // the segment tree
    vector<F> pends;   // stores pending updates for lazy propogation

    lli segtree_size(lli n) {
        n -= 1;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return 2 * n + 1;
    }

public:
    explicit SegTree(lli N):
        n(N), values(segtree_size(n)), pends(values.size()) {identity_check();}  // creates empty segment tree
    explicit SegTree(lli N, const M& x):
        n(N), values(segtree_size(n)), pends(values.size()) {identity_check(); assign(x);}  // segment tree with fixed leaf values 
    explicit SegTree(std::vector<M>& v):
        n(v.size()), values(segtree_size(v.size())), pends(values.size()) {identity_check(); assign(v);}

private:
    // build function with vector of leaves 
    void assign_values(lli root, lli first, lli last, vector<M> &a) {
        // root has the node number, first and last have the array indices.
        if(first == last) {
            values[root] = a[first];
        }
        else {
            lli left = 2 * root + 1;
            lli mid = (first + last) / 2;
            assign_values(left, first, mid, a);
            assign_values(left + 1, mid + 1, last, a);
            values[root] = M(values[left], values[left + 1]);
        }
    }

    // build function with every leaf assigned x
    void assign_values(lli root, lli first, lli last, M& x) {
        // root has the node number, first and last have the array indices.
        if(first == last) {
            values[root] = x;
        }
        else {
            lli left = 2 * root + 1;
            lli mid = (first + last) / 2;
            assign_values(left, first, mid, x);
            assign_values(left+1, mid+1, last, x);
            values[root] = M(values[left], values[left+1]);
        }
    }

public:
    // assign every leaf x
    void assign(M& x) {
        pends.assign(values.size(), F());
        assign_values(0, 0, n - 1, x);
    }

    // assign every leaf corresponding vector entry
    void assign(vector<M>& v) {
        if(v.size() < n) {
            std::exit(3);
        }
        assign_values(0, 0, n - 1, v);
    }

private:
    // lazy propagation
    void propagate(lli root, lli first, lli last) {
        if(!pends[root].is_identity()) {
            values[root] = pends[root](values[root]);
            if(first != last)
            {
                lli left = 2 * root + 1;
                pends[left] = F(pends[root], pends[left]);
                pends[left + 1] = F(pends[root], pends[left + 1]);
            }
            pends[root] = F();
        }
    }
    // query from range l to r (zero indexing)
    M query(lli root, lli first, lli last, lli l, lli r) {
        if(l > last or r < first) {
            return M();
        }
        propagate(root, first, last);
        if(l <= first and last <= r) {
            return values[root];
        }
        else {
            lli left = 2 * root + 1;
            lli mid = (first + last) / 2;
            return M(query(left, first, mid, l, r), query(left + 1, mid + 1, last, l, r));
        }
    }

public:
    // query from range l to r (zero indexing)
    M query(lli l, lli r) {
        return query(0, 0, n - 1, l, r);
    }

private:
    // update from range l to r by giving update function (zero indexing)
    void update(lli root, lli first, lli last, lli l, lli r, const F& f) {
        if(l > last or r < first) {
            propagate(root, first, last);
        }
        else if(l <= first and last <= r) {
            pends[root] = F(f, pends[root]);
            propagate(root, first, last);
        }
        else {
            propagate(root, first, last);
            lli left = 2 * root + 1;
            lli mid = (first + last) / 2;
            update(left, first, mid, l, r, f);
            update(left + 1, mid + 1, last, l, r, f);
            values[root] = M(values[left], values[left + 1]);
        }
    }

public:
    // update from range l to r by giving update function (zero indexing)
    void update(lli l, lli r, const F& f) {
        update(0, 0, n - 1, l, r, f);
    }
};
