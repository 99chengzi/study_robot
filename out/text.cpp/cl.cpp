#include <bits/stdc++.h>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        int max_val = 0, min_val = 1e18;
        for (int i = 0; i < n; ++i)
        {
            int x;
            cin >> x;
            if (i == 0)
            {
                max_val = min_val = x;
            }
            else
            {
                max_val = max(max_val, x);
                min_val = min(min_val, x);
            }
        }
        cout << max_val - min_val << endl;
    }
    return 0;
}