#include <iostream>
#include <list>
#include <cmath>

using namespace std;

class Graf
{
    int liczbaWierzcholkow;
    float** macierzWag;
public:
    bool czyWagiNieujemne = true;
    float* FordBellman(int zrodlo);
    list<int> FordBellman(int zrodlo, int cel);
    Graf(int liczbaWierzcholkow);
    ~Graf();
    void dodajKrawedz(int wierzcholek1, int wierzcholek2, float waga);
};

Graf::Graf(int liczbaWierzcholkow)
{
    this->liczbaWierzcholkow = liczbaWierzcholkow;
    macierzWag = new float* [liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        macierzWag[i] = new float[liczbaWierzcholkow];
        for (int j = 0; j < liczbaWierzcholkow; j++)
        {
            if (i == j)
                macierzWag[i][j] = 0;
            else
                macierzWag[i][j] = INFINITY;
        }
    }
}

Graf::~Graf()
{
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        delete[] macierzWag[i];
    }
    delete[] macierzWag;
}

void Graf::dodajKrawedz(int wierzcholek1, int wierzcholek2, float waga = 1)
{
    if (waga < 0) czyWagiNieujemne = false;
    macierzWag[wierzcholek1][wierzcholek2] = waga;
}

float* Graf::FordBellman(int zrodlo)
{
    int s = zrodlo - 1;
    int n = liczbaWierzcholkow;
    float** A = macierzWag;
    float* D = new float[n];

    for (int v = 0; v < n; v++)
    {
        D[v] = A[s][v];
    }
    D[s] = 0;
    for (int k = 0; k < n - 2; k++)
    {
        for (int v = 0; v < n; v++)
        {
            if (v != s)
            {
                for (int u = 0; u < n; u++)
                {
                    if (D[v] > D[u] + A[u][v])
                    {
                        D[v] = D[u] + A[u][v];
                    }
                }
            }
        }
        //wyświetlanie iteracji
        //for (int i = 0; i < liczbaWierzcholkow; i++)
        //    cout << D[i] << '\t';
        //cout << endl;
        //
    }
    return D;
}

list<int> Graf::FordBellman(int zrodlo, int cel)
{
    int s = zrodlo - 1;
    int t = cel - 1;
    int n = liczbaWierzcholkow;
    float** A = macierzWag;
    float* D = new float[n];
    int* P = new int[n]; //tablica poprzedników
    list<int> sciezka;

    for (int v = 0; v < n; v++)
    {
        D[v] = INFINITY;
        P[v] = -1;
    }
    D[s] = 0;
    for (int k = 0; k < n - 2; k++)
    {
        for (int v = 0; v < n; v++)
        {
            if (v != s)
            {
                for (int u = 0; u < n; u++)
                {
                    if (D[v] > D[u] + A[u][v])
                    {
                        D[v] = D[u] + A[u][v];
                        P[v] = u;
                    }
                }
            }
        }
        //wyświetlanie iteracji
        //for (int i = 0; i < liczbaWierzcholkow; i++)
        //    cout << D[i] << '\t';
        //cout << endl;
        //for (int i = 0; i < liczbaWierzcholkow; i++)
        //    cout << P[i] << '\t';
        //cout << endl;
        //
    }

    if (P[t] == -1)
        sciezka.push_front(-1);
    else
    {
        int poprzednik = t;
        sciezka.push_front(poprzednik + 1);
        while (poprzednik != s)
        {
            poprzednik = P[poprzednik];
            sciezka.push_front(poprzednik + 1);
        }
    }
    return sciezka;
}

int main()
{
    int liczbaZestawow;
    cin >> liczbaZestawow;

    for (int i = 1; i <= liczbaZestawow; i++)
    {
        int liczbaWierzcholkow;
        cin >> liczbaWierzcholkow;
        int liczbaKrawedzi;
        cin >> liczbaKrawedzi;
        Graf graf(liczbaWierzcholkow);
        //pobieranie danych do list sąsiedztwa
        for (int j = 0; j < liczbaKrawedzi; j++)
        {
            int wierzcholek1, wierzcholek2, waga = 1;
            cin >> wierzcholek1;
            cin >> wierzcholek2;
            cin >> waga;
            graf.dodajKrawedz(wierzcholek1 - 1, wierzcholek2 - 1, waga);
        }

        cout << "Graf nr " << i;
        float* wagiSciezek;
        wagiSciezek = graf.FordBellman(1);
        for (int j = 2; j <= liczbaWierzcholkow; j++)
        {
            cout << endl;
            if (wagiSciezek[j - 1] == INFINITY)
            {
                cout << "NIE ISTNIEJE DROGA Z 1 DO " << j;
            }
            else
            {
                list<int> sciezka;
                sciezka = graf.FordBellman(1, j);
                for (auto it = sciezka.begin(); it != sciezka.end(); it++)
                    cout << *it << '-';
                cout << '\b' << ' ';
                cout << wagiSciezek[j - 1];
            }
        }
        delete[] wagiSciezek;
        if (i < liczbaZestawow)
            cout << endl;
    }
}