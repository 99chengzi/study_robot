#include <bits/stdc++.h>
using namespace std;

int main()
{
    string a, b; // a,b表示两个数
                 // n表示a的位数,m表示b的位数
    cin >> a >> b;
    string ans;
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    int jinwei = 0;
    int i = 0;

    while (i < a.size() || i < b.size() || jinwei > 0)
    {
        int num1 = (i < a.size()) ? (a[i] - '0') : 0;
        int num2 = (i < b.size()) ? (b[i] - '0') : 0;

        int sum = num1 + num2 + jinwei;

        ans.push_back((sum % 10) + '0');

        jinwei = sum / 10;

        i++;
    }
    reverse(ans.begin(), ans.end());
    cout << ans << endl;
}