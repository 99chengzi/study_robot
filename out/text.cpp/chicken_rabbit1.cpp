#include <bits/stdc++.h>
using namespace std;

int main()
{
    int foot, head;
    cin >> head >> foot;
    int rabbit = foot / 2 - head;
    int chicken = head - rabbit;
    cout << chicken << " " << rabbit << endl;
    return 0;
}