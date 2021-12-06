#include <iostream>
#include <list>
#include <numeric>
#include <stack>
#include <cmath>
#include <queue>
#include <limits.h>

using namespace std;

class Graf
{
    int liczbaWierzcholkow;
    float** macierzWag;
    int s;
    int t;
    bool BFS(int** grafRezydualny, int* poprzednik);
public:
    void wyswietlMacierzWag();
    Graf(int liczbaWierzcholkow);
    ~Graf();
    void dodajKrawedz(int wierzcholek1, int wierzcholek2, float waga);
    void usunKrawedz(int wierzcholek1, int wierzcholek2);
    int fordFulkerson();
    void krawedzieDoZmniejszenia();
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
            macierzWag[i][j] = 0;
        }
    }
    s = 0;
    t = liczbaWierzcholkow - 1;
}

Graf::~Graf()
{
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        delete[] macierzWag[i];
    }
    delete[] macierzWag;
}

void Graf::wyswietlMacierzWag()
{
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        for (int j = 0; j < liczbaWierzcholkow; j++)
        {
            cout << macierzWag[i][j] << '\t';
        }
        cout << endl;
    }
}

void Graf::dodajKrawedz(int wierzcholek1, int wierzcholek2, float waga = 1)
{
    macierzWag[wierzcholek1 - 1][wierzcholek2 - 1] = waga;
}

void Graf::usunKrawedz(int wierzcholek1, int wierzcholek2)
{
    macierzWag[wierzcholek1 - 1][wierzcholek2 - 1] = 0;
}

bool Graf::BFS(int** grafRezydualny, int* poprzednik)
{
    bool* odwiedzone = new bool[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        odwiedzone[i] = false;
    }

    queue <int> kolejka;
    kolejka.push(s);
    odwiedzone[s] = true;
    poprzednik[s] = -1;

    while (!kolejka.empty())
    {
        int u = kolejka.front();
        kolejka.pop();

        for (int v = 0; v < liczbaWierzcholkow; v++)
        {
            if (odwiedzone[v] == false && grafRezydualny[u][v] > 0)
            {
                kolejka.push(v);
                poprzednik[v] = u;
                odwiedzone[v] = true;
            }
        }
    }

    bool czySciezkaDoUjscia = false;
    if (odwiedzone[t] == true)
        czySciezkaDoUjscia = true;
    delete[] odwiedzone;

    return (czySciezkaDoUjscia);
}

int Graf::fordFulkerson()
{
    int u, v;

    int** grafRezydualny = new int* [liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        grafRezydualny[i] = new int[liczbaWierzcholkow];

    for (u = 0; u < liczbaWierzcholkow; u++)
        for (v = 0; v < liczbaWierzcholkow; v++)
            grafRezydualny[u][v] = macierzWag[u][v];

    int* poprzednik = new int[liczbaWierzcholkow];

    int max_flow = 0;

    while (BFS(grafRezydualny, poprzednik))
    {
        int path_flow = INT_MAX;
        for (v = t; v != s; v = poprzednik[v])
        {
            u = poprzednik[v];
            path_flow = min(path_flow, grafRezydualny[u][v]);
        }

        for (v = t; v != s; v = poprzednik[v])
        {
            u = poprzednik[v];
            grafRezydualny[u][v] -= path_flow;
            grafRezydualny[v][u] += path_flow;
        }

        max_flow += path_flow;
    }
    //wyswietlanie
    //cout << "Graf rezydualny:" << endl;
    //for (int i = 0; i < liczbaWierzcholkow; i++)
    //{
    //    for (int j = 0; j < liczbaWierzcholkow; j++)
    //    {
    //        cout << grafRezydualny[i][j] << '\t';
    //    }
    //    cout << endl;
    //}
    //
    //cout << "Przepływy:" << endl;
    //for (int i = 0; i < liczbaWierzcholkow; i++)
    //{
    //    for (int j = 0; j < liczbaWierzcholkow; j++)
    //    {
    //        if (macierzWag[i][j] - grafRezydualny[i][j] > 0) cout << macierzWag[i][j] - grafRezydualny[i][j] << '\t';
    //        else cout << 0 << '\t';
    //    }
    //    cout << endl;
    //}
    //

    for (int i = 0; i < liczbaWierzcholkow; i++)
        delete[] grafRezydualny[i];
    delete[] grafRezydualny;

    return max_flow;
}

void Graf::krawedzieDoZmniejszenia()
{
    int maxFlow = fordFulkerson();
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        for (int j = 0; j < liczbaWierzcholkow; j++)
        {
            if (macierzWag[i][j] > 0)
            {
                int waga = macierzWag[i][j];
                dodajKrawedz(i + 1, j + 1, waga - 1);
                if (fordFulkerson() >= maxFlow)
                {
                    int oIle = 0;
                    while (fordFulkerson() >= maxFlow)
                    {
                        oIle++;
                        int wagaZmieniana = macierzWag[i][j];
                        if (wagaZmieniana == 0) break;
                        dodajKrawedz(i + 1, j + 1, wagaZmieniana - 1);
                    }
                    cout << i + 1 << "-" << j + 1 << " o " << oIle << endl;
                }
                dodajKrawedz(i + 1, j + 1, waga);
            }
        }
    }
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
        //pobieranie danych
        for (int i = 0; i < liczbaKrawedzi; i++)
        {
            int wierzcholek1;
            cin >> wierzcholek1;
            int wierzcholek2;
            cin >> wierzcholek2;
            int waga;
            cin >> waga;
            graf.dodajKrawedz(wierzcholek1, wierzcholek2, waga);
        }

        cout << "Siec nr " << i << endl;
        graf.krawedzieDoZmniejszenia();
    }
}
