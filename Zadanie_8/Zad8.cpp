#include <iostream>
#include <string>

using namespace std;

int main()
{
    int liczbaZestawow;
    cin >> liczbaZestawow;

    for (int numerZestawu = 1; numerZestawu <= liczbaZestawow; numerZestawu++)
    {
        string wzorzec;
        cin >> wzorzec;
        string tekst;
        cin >> tekst;

        int Zp = (int)"0"[0];
        int Zk = (int)"9"[0];
        int* Last = new int[Zk - Zp + 1];
        for (int j = 0; j <= Zk - Zp; j++)
            Last[j] = -1;
        for (int j = 0; j < wzorzec.length(); j++)
        {
            Last[(int)(wzorzec[j]) - Zp] = j;
        }

        //Boyer-Moore
        int m = wzorzec.length();
        int n = tekst.length();
        int pp = -1;
        int i = 0;
        while (i <= n - m)
        {
            int j = m - 1;
            while ((j > -1) && (wzorzec[j] == tekst[i + j]))
            {
                j = j - 1;
            }
            if (j <= -1)
            {
                pp = i;
                cout << pp << ' ';
                i = i + 1;
            }
            else
            {
                i = i + max(1, j - Last[(int)(tekst[i + j]) - Zp]);
            }
        }

        if (pp == -1) cout << "BRAK";
        cout << endl;
        delete[] Last;
    }
}
