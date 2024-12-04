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

// Constants
const int MOD = 1e9 + 7;
const ll INF = 1e18;
const double EPS = 1e-9;
const double PI = acos(-1);

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

// Binary exponentiation (for non-modular calculations)
ll binpow(ll base, ll exp)
{
    ll res = 1;
    while (exp > 0)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/************/

bool check(int x)
{

    return !(x > 0 && x < 10);
}

bool valid(string str)
{
    int sum = 0;
    forn(i, str.size())
    {

        if (str[i] = '(')
            sum++;
        else
            sum--;

        if (sum < 0)
            return false;
    }

    return true;
}
void solve()
{

    int r1, r2, c1, c2, d1, d2;
    cin >> r1 >> r2 >> c1 >> c2 >> d1 >> d2;

    int res = (r1 + r2 + c1 + c2 + d1 + d2) / 3;

    if ((r1 + r2) != res || (c1 + c2) != res || (d1 + d2) != res)
    {
        cout << -1 << nline;
        return;
    }

    int a = (r1 - d2 + c1) / 2;
    int c = (r2 - d1 + c1) / 2;
    int b = (r1 - c1 + d2) / 2;
    int d = (r2 - c1 + d1) / 2;

    set<int> s;
    s.insert(a);
    s.insert(b);
    s.insert(c);
    s.insert(d);

    if (s.size() != 4 || check(a) || check(b) || check(c) || check(d))
    {
        cout << -1 << nline;
        return;
    }

    cout << a << " " << b << nline << c << " " << d << nline;
}

int main()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

#ifndef ONLINE_JUDGE
    freopen("./outputs/input.txt", "r", stdin);
    freopen("./outputs/output.txt", "w", stdout);
#endif

    // int t;
    // cin >> t;
    // while (t--)
    solve();

    return 0;
}
// Explanation

/*
* after manual calculation, we calculate that the sum of all values, will be equal
* 1/3 times of each (r1+r2),(c1+c2) and (d1+d2)
* we create 4 equeation for a,b,c,d and calculate values, if (values are repetetive or if the 
* number is not in range 1-9 ,  we return -1

*/