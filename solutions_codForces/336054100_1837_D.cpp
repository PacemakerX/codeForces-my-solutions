#include <bits/stdc++.h>
using namespace std;

// Type definitions
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vpii;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vl> vvl;

// Macros
#define nline "\n"
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define forn(i, n) for (int i = 0; i < int(n); i++)
#define forr(i, a, b) for (int i = a; i <= b; i++)
#define ford(i, a, b) for (int i = a; i >= b; i--)
#define elasped_time 1.0 * clock() / CLOCKS_PER_SEC

// clang-format off
// Debug macros
#define debarr(a,n) cout<<#a<<" : ";for(int i=0;i<n;i++) cerr<<a[i]<<" "; cerr<<endl;
#define debmat(mat,row,col) cout<<#mat<<" :\n";for(int i=0;i<row;i++) {for(int j=0;j<col;j++) cerr<<mat[i][j]<<" ";cerr<<endl;}
#define pr(...) dbs(#__VA_ARGS__, __VA_ARGS__)

// Debug template functions
template <class S, class T>ostream& operator <<(ostream& os, const pair<S, T>& p) {return os << "(" << p.first << ", " << p.second << ")";}
template <class T>ostream& operator <<(ostream& os, const vector<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T>ostream& operator <<(ostream& os, const unordered_set<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class S, class T>ostream& operator <<(ostream& os, const unordered_map<S, T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T>ostream& operator <<(ostream& os, const set<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T>ostream& operator <<(ostream& os, const multiset<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class S, class T>ostream& operator <<(ostream& os, const map<S, T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T> void dbs(string str, T t) {cerr << str << " : " << t << "\n";}
template <class T, class... S> void dbs(string str, T t, S... s) {int idx = str.find(','); cerr << str.substr(0, idx) << " : " << t << ","; dbs(str.substr(idx + 1), s...);}
template <class T> void prc(T a, T b) {cerr << "["; for (T i = a; i != b; ++i) {if (i != a) cerr << ", "; cerr << *i;} cerr << "]\n";}
// clang-format on

// Constants
const int MOD = 1e9 + 7;
const ll INF = 1e18;
const double EPS = 1e-9;
const double PI = acos(-1);

// Utility functions
ll mod_add(ll a, ll b, ll m = MOD) { return ((a % m) + (b % m)) % m; }
ll mod_sub(ll a, ll b, ll m = MOD) { return ((a % m) - (b % m) + m) % m; }
ll mod_mul(ll a, ll b, ll m = MOD) { return ((a % m) * (b % m)) % m; }
ll mod_pow(ll base, ll exp, ll m = MOD)
{
    ll res = 1;
    base %= m;
    while (exp > 0)
    {
        if (exp & 1)
            res = mod_mul(res, base, m);
        base = mod_mul(base, base, m);
        exp >>= 1;
    }
    return res;
}
ll mod_inv(ll a, ll m = MOD)
{
    return mod_pow(a, m - 2, m); // Only works if m is prime
}
ll mod_div(ll a, ll b, ll m = MOD)
{
    return mod_mul(a, mod_inv(b, m), m);
}

ll binpow(ll base, ll exp, ll mod = MOD)
{
    ll res = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}
/************/
bool mainCheck(string &str)
{
    int n = str.size();

    int cnt = 0;

    forn(i, n)
    {
        cnt += (str[i] == '(' ? 1 : -1);
    }

    return cnt != 0;
}
bool check(string &str)
{

    stack<char> st;

    int n = str.size();
    for (int i = 0; i < n; i++)
    {
        if (!st.empty())
        {
            if (str[i] == '(' && st.top() == ')')
                st.pop();
            else if (str[i] == ')' && st.top() == '(')
                return true;
            else
                st.push(str[i]);
        }
        else
            st.push(str[i]);
    }

    return false;
}
void solve()
{
    int n;
    cin >> n;
    string str;
    cin >> str;

    if ((n & 1) || mainCheck(str))
    {
        cout << -1 << nline;
        return;
    }

    vi arr(n, 0);

    bool used1 = false, used2 = false;
    ll cst = 0;
    ll i = 0;

    bool flag = check(str);
    while (i < n)
    {
        while (i < n && (cst > 0 || str[i] == '('))
        {
            cst += (str[i] == '(' ? 1 : -1);
            arr[i++] = 1;
            used1 = true;
        }

        while (i < n && (cst < 0 || str[i] == ')'))
        {
            cst += (str[i] == '(' ? 1 : -1);
            if (!flag)
                arr[i++] = 1;
            else
            {
                arr[i++] = 2;
                used2 = true;
            }
        }
    }

    cout << ((flag && used1 && used2) ? 2 : 1) << nline;
    for (auto i : arr)
        cout << i << " ";

    cout << nline;
}
int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("./outputs/input.txt", "r", stdin);
    freopen("./outputs/output.txt", "w", stdout);
    cerr.rdbuf(cout.rdbuf());
#endif

    int t;
    cin >> t;
    // int t = 1;
    while (t--)
        solve();

    return 0;
}
