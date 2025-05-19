#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

using namespace std;
void bitonicmerge(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
        for (int i = low; i < (low + k); i++)
        {
            if (dir == (arry[i] > arry[i + k]))
            {
                int temp = arry[i];
                arry[i] = arry[i + k];
                arry[i + k] = temp;
            }
        }
        bitonicmerge(arry, low, k, dir);
        bitonicmerge(arry, low + k, k, dir);
    }
}
void bitonicsort(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
        bitonicsort(arry, low, k, 1);
        bitonicsort(arry, low + k, k, 0);
        bitonicmerge(arry, low, count, dir);
    }
}

int main()
{
    vector<int> arr = {3, 6, 5, 7, 4, 1, 8, 2};
    string out = "";
    for (int i : arr)
    {
        out = out + to_string(i) + ",";
    }

    cout << "Array before sort: " << out << endl;
    bitonicsort(arr, 0, arr.size(), 1);
    out = "";
    for (int i : arr)
    {
        out = out + to_string(i) + ",";
    }

    cout << "Array after sort: " << out;
    return 0;
}
