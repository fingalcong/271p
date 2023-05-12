#include<iostream>
#include<algorithm>
#include<cstdio>
#include<queue>
const double INF = 100000.0;
const int MAX_N = 1001;
using namespace std;
int n;
double cost[MAX_N][MAX_N];
struct Node
{
    bool visited[MAX_N];
    int s;
    double s_p;
    int e;
    double e_p;
    double k;
    double sumv;
    double lb;
    bool operator <(const Node& p)const
    {
        return p.lb < lb;
    }
};
priority_queue<Node> pq;
double low, up;
bool dfs_visited[MAX_N];

double dfs(int u, int k, double l)
{
    if (k == n) return l + cost[u][1];
    int minlen = INF, p;
    for (int i = 1; i <= n; i++)
    {
        if (!dfs_visited[i] && minlen > cost[u][i])
        {
            minlen = cost[u][i];
            p = i;
        }
    }
    dfs_visited[p] = true;
    return dfs(p, k + 1, l + minlen);
}
void get_up()
{
    dfs_visited[1] = true;
    up = dfs(1, 1, 0.0);
}
void get_low()
{
    low = 0.0;
    for (int i = 1; i <= n; i++)
    {
        double tmpA[MAX_N];
        for (int j = 1; j <= n; j++)
        {
            tmpA[j] = cost[i][j];
        }
        sort(tmpA + 1, tmpA + 1 + n);
        low += tmpA[1];
    }
}
double get_lb(Node p)
{
    double ret = p.sumv * 2;
    double min1 = INF, min2 = INF;
    for (int i = 1; i <= n; i++)
    {
        if (!p.visited[i] && min1 > cost[i][p.s])
        {
            min1 = cost[i][p.s];
        }
    }
    ret += min1;
    for (int i = 1; i <= n; i++)
    {
        if (!p.visited[i] && min2 > cost[p.e][i])
        {
            min2 = cost[p.e][i];
        }
    }
    ret += min2;
    for (int i = 1; i <= n; i++)
    {
        if (!p.visited[i])
        {
            min1 = min2 = INF;
            for (int j = 1; j <= n; j++)
            {
                if (min1 > cost[i][j])
                    min1 = cost[i][j];
            }
            for (int j = 1; j <= n; j++)
            {
                if (min2 > cost[j][i])
                    min2 = cost[j][i];
            }
            ret += min1 + min2;
        }
    }
    return (ret + 1) / 2;
}

double solve()
{
    get_up();
    get_low();

    Node star;
    star.s = 1;
    star.e = 1;
    star.k = 1;
    for (int i = 1; i <= n; i++)
    {
        star.visited[i] = false;
    }
    star.visited[1] = true;
    star.sumv = 0.0;
    star.lb = low;   
    double ret = INF;
    pq.push(star);
    while (pq.size())
    {

        Node tmp = pq.top();pq.pop();
        if (tmp.k == n - 1)
        {
            int p;
            for (int i = 1; i <= n; i++)
            {
                if (!tmp.visited[i])
                {
                    p = i;
                    break;
                }
            }
            double ans = tmp.sumv + cost[p][tmp.s] + cost[tmp.e][p];
            if (ans <= tmp.lb)
            {
                ret = min(ans, ret);
                break;
            }
            else
            {
                up = min(up, ans);
                ret = min(ret, ans);
                continue;
            }
        }
        Node next;
        for (int i = 1; i <= n; i++)
        {
            if (!tmp.visited[i])
            {
                next.s = tmp.s;           
                next.sumv = tmp.sumv + cost[tmp.e][i];             
                next.e = i;               
                next.k = tmp.k + 1;              
                for (int j = 1; j <= n; j++) next.visited[j] = tmp.visited[j];
                next.visited[i] = true;
                next.lb = get_lb(next);
                if (next.lb > up) continue;
                pq.push(next);
            }
        }
    }
    return ret;
}
int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> cost[i][j];
            if (i == j)
            {
                cost[i][j] = INF;
            }
        }
    }
    cout << solve() << endl;
    return 0;
}