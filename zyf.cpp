#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int a[110];
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i];
    }
    int cnt[100000] = {0}; // 初始化数组里所有的数出现的次数为0
    for (int i = 0; i < n; ++i)
    {
        cnt[a[i]]++; // 数字i每出现一次次数就加1
    }
    int max = 0;
    int num_max = 0;
    for (int i = 0; i < n; ++i)
    {
        if (cnt[a[i]] == 0)
        {
            continue;
        } // 如果哪个数字出现的次数为0次，就直接跳出循环
        if (cnt[a[i]] > max)
        {
            max = cnt[a[i]];
            num_max = a[i];
        }
    }
    int min = 10000; // 初始化出现的次数最少的次数很大
    int num_min = 0; // 用来记录出现次数最多的数是多少
    for (int i = 0; i < n; ++i)
    {
        if (cnt[a[i]] == 0)
        {
            continue;
        }
        if (cnt[a[i]] < min)
        {
            min = cnt[a[i]];
            num_min = a[i]; // 第i个数字
        }
    }
    cout << num_max << " " << num_min << endl;
    return 0;
}
