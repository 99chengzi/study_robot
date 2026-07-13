#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n, m;
    cin >> n >> m;
    if (m % 2 == 0)
    {
        int rabbit = (m - 2 * n) / 2;
        int chicken = n - rabbit;
        if (rabbit >= 0 && chicken >= 0)
        {
            cout << chicken << " " << rabbit << endl;
        }
        else
        {
            cout << "-1 -1" << endl;
        }
    }
    else
    {
        cout << "-1 -1" << endl;
    }
    return 0;
}