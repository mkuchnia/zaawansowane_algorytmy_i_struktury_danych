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
    Graf wydzielSiecWarstwowa();
    int fordFulkerson();
    void krawedzieDoZwiekszenia();
    int edmondsKarp();
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

Graf Graf::wydzielSiecWarstwowa()
{
    list<list<int>> listaWarstw;
    listaWarstw.push_back(list<int>());
    listaWarstw.back().push_back(s);
    while (listaWarstw.back().back() != t)
    {
        list<int> nowaWarstwa;
        for (auto it = listaWarstw.back().begin(); it != listaWarstw.back().end(); it++)
        {
            if (!nowaWarstwa.empty())
                if (nowaWarstwa.back() == t)
                    break;
            int wierzcholek = *it;
            for (int j = 0; j < liczbaWierzcholkow; j++)
            {
                if (macierzWag[wierzcholek][j] != 0 && macierzWag[wierzcholek][j] != INFINITY)
                {
                    if (j == t)
                    {
                        nowaWarstwa.clear();
                    }
                    //nie dodawaj jeśli łączy się z tą samą warstwą
                    nowaWarstwa.push_back(j);
                    cout << "Dodano wierzcholek: " << j + 1 << " z wierzcholka: " << wierzcholek + 1 << endl;
                }
            }
        }
        nowaWarstwa.sort();
        nowaWarstwa.unique();
        for (auto it = listaWarstw.back().begin(); it != listaWarstw.back().end(); it++)
        {
            for (auto it2 = nowaWarstwa.begin(); it2 != nowaWarstwa.end(); it2++)
            {
                if (*it == *it2)
                {
                    it2 = nowaWarstwa.erase(it2);
                }
            }
        }
        listaWarstw.push_back(nowaWarstwa);
    }

    for (auto it = listaWarstw.begin(); it != listaWarstw.end(); it++)
    {
        cout << "warstwa: " << '\t';
        for (auto it2 = it->begin(); it2 != it->end(); it2++)
        {
            cout << *it2 + 1 << '\t';
        }
        cout << endl;
    }

    return Graf(1);
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

int Graf::edmondsKarp()
{
    int max_flow = 0;
    int** grafRezydualny = new int* [liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        grafRezydualny[i] = new int[liczbaWierzcholkow];
    for (int u = 0; u < liczbaWierzcholkow; u++)
        for (int v = 0; v < liczbaWierzcholkow; v++)
            grafRezydualny[u][v] = macierzWag[u][v];
    int* poprzednik = new int[liczbaWierzcholkow];
    int new_flow;

    while (new_flow = BFS(grafRezydualny, poprzednik)) {
        max_flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = poprzednik[cur];
            grafRezydualny[prev][cur] -= new_flow;
            grafRezydualny[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return max_flow;
}

void Graf::krawedzieDoZwiekszenia()
{
    int maxFlow = fordFulkerson();
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        for (int j = 0; j < liczbaWierzcholkow; j++)
        {
            if (macierzWag[i][j] > 0)
            {
                int waga = macierzWag[i][j];
                dodajKrawedz(i + 1, j + 1, waga+1);
                if (fordFulkerson() > maxFlow)
                {
                    cout << i + 1 << "-" << j + 1 << endl;
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
        graf.krawedzieDoZwiekszenia();
    }
}
