#include <iostream>
#include <list>
#include <numeric>
#include <queue>

using namespace std;

class Graf
{
    int liczbaWierzcholkow;
    list<int>* listaSasiedztwa;
    list<int>* listaWag;
    bool czyCykleRoboczy(int wierzcholek, bool odwiedzone[], int rodzic);
public:
    Graf(int liczbaWierzcholkow);
    Graf(const Graf& grafKopiowany);
    void dodajKrawedz(int wierzcholek1, int wierzcholek2, int waga);
    bool usunKrawedz(int wierzcholek1, int wierzcholek2);
    bool czyCykle();
    int sumaWag();
    int liczbaKrawedzi();
    Graf MSTkruskal();
    Graf MSTprim();
};

Graf::Graf(int liczbaWierzcholkow)
{
    this->liczbaWierzcholkow = liczbaWierzcholkow;
    listaSasiedztwa = new list<int>[liczbaWierzcholkow];
    listaWag = new list<int>[liczbaWierzcholkow];
}

Graf::Graf(const Graf& grafKopiowany)
{
    liczbaWierzcholkow = grafKopiowany.liczbaWierzcholkow;
    listaSasiedztwa = new list<int>[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        listaSasiedztwa[i] = list<int>(grafKopiowany.listaSasiedztwa[i]);
    listaWag = new list<int>[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        listaWag[i] = list<int>(grafKopiowany.listaWag[i]);
}

void Graf::dodajKrawedz(int wierzcholek1, int wierzcholek2, int waga = 1)
{
    listaSasiedztwa[wierzcholek1].push_back(wierzcholek2);
    listaWag[wierzcholek1].push_back(waga);
}

bool Graf::usunKrawedz(int wierzcholek1, int wierzcholek2)
{
    try
    {
        list<int>::iterator it = find(listaSasiedztwa[wierzcholek1].begin(), listaSasiedztwa[wierzcholek1].end(), wierzcholek2);
        int pozycja = distance(listaSasiedztwa[wierzcholek1].begin(), it);
        listaSasiedztwa[wierzcholek1].erase(it);
        it = listaWag[wierzcholek1].begin();
        advance(it, pozycja);
        listaWag[wierzcholek1].erase(it);
        return true;
    }
    catch (exception e)
    {
        return false;
    }
}

bool Graf::czyCykleRoboczy(int wierzcholek, bool odwiedzone[], int rodzic)
{
    odwiedzone[wierzcholek] = true;

    list<int>::iterator i;
    for (i = listaSasiedztwa[wierzcholek].begin(); i != listaSasiedztwa[wierzcholek].end(); ++i)
    {
        if (odwiedzone[*i] != true)
        {
            if (czyCykleRoboczy(*i, odwiedzone, wierzcholek))
                return true;
        }
        else if (*i != rodzic)
            return true;
    }
    return false;
}

bool Graf::czyCykle()
{
    bool* odwiedzone = new bool[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        odwiedzone[i] = false;

    for (int u = 0; u < liczbaWierzcholkow; u++)
    {
        if (odwiedzone[u] != true)
            if (czyCykleRoboczy(u, odwiedzone, -1))
                return true;
    }
    return false;
}

int Graf::sumaWag()
{
    int suma = 0;
    for (int i = 0; i < liczbaWierzcholkow; i++)
        suma += accumulate(begin(listaWag[i]), end(listaWag[i]), 0);
    return suma;
}

int Graf::liczbaKrawedzi()
{
    int suma = 0;
    for (int i = 0; i < liczbaWierzcholkow; i++)
        suma += listaSasiedztwa[i].size();
    return suma;
}

Graf Graf::MSTkruskal()
{
    Graf staryGraf(*this);
    Graf nowyGraf(liczbaWierzcholkow);
    priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>* kolejkaSasiedztwa;
    kolejkaSasiedztwa = new priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        list<int>::iterator itSasiedztwa = listaSasiedztwa[i].begin();
        list<int>::iterator itWag = listaWag[i].begin();
        while (itSasiedztwa != listaSasiedztwa[i].end())
        {
            kolejkaSasiedztwa[i].push(make_pair(*itWag, *itSasiedztwa));
            advance(itSasiedztwa, 1);
            advance(itWag, 1);
        }
    }

    while (nowyGraf.liczbaKrawedzi() / 2 < liczbaWierzcholkow - 1 && staryGraf.liczbaKrawedzi() / 2 > 0)
    {
        int e1 = NULL, e2 = NULL, eW = NULL;
        for (int i = 0; i < liczbaWierzcholkow; i++)
        {
            if (!kolejkaSasiedztwa[i].empty())
            {
                if (kolejkaSasiedztwa[i].top().first < eW || eW == NULL)
                {
                    eW = kolejkaSasiedztwa[i].top().first;
                    e1 = i;
                    e2 = kolejkaSasiedztwa[i].top().second;
                }
            }
        }
        kolejkaSasiedztwa[e1].pop();
        kolejkaSasiedztwa[e2].pop();
        staryGraf.usunKrawedz(e1, e2);
        staryGraf.usunKrawedz(e2, e1);

        nowyGraf.dodajKrawedz(e1, e2, eW);
        nowyGraf.dodajKrawedz(e2, e1, eW);
        if (nowyGraf.czyCykle())
        {
            nowyGraf.usunKrawedz(e1, e2);
            nowyGraf.usunKrawedz(e2, e1);
        }
        //else
        //{
        //    cout << "Dodano krawedz: " << e1 << ' ' << e2 << ' '  << eW << endl;
        //}
    }
    if (nowyGraf.liczbaKrawedzi() / 2 < liczbaWierzcholkow - 1)
        return false;
    else
        return nowyGraf;
}

Graf Graf::MSTprim()
{
    Graf nowyGraf(liczbaWierzcholkow);
    priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>* kolejkaSasiedztwa;
    kolejkaSasiedztwa = new priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>[liczbaWierzcholkow];

    int wierzcholekStartowy = 0;
    vector<int> wierzcholkiWGrafie;
    wierzcholkiWGrafie.push_back(wierzcholekStartowy);

    //jeśli wierzchołek jest stopnia 0 to graf jest niespójny
    if (listaSasiedztwa[wierzcholekStartowy].empty()) return false;
    //zaktualizuj kolejke
    list<int>::iterator itSasiedztwa = listaSasiedztwa[wierzcholekStartowy].begin();
    list<int>::iterator itWag = listaWag[wierzcholekStartowy].begin();
    while (itSasiedztwa != listaSasiedztwa[wierzcholekStartowy].end())
    {
        kolejkaSasiedztwa[wierzcholekStartowy].push(make_pair(*itWag, *itSasiedztwa));
        advance(itSasiedztwa, 1);
        advance(itWag, 1);
    }
    //dopóki graf nie będzie zawierał wszystkich wierzchołków 
    while (wierzcholkiWGrafie.size() < liczbaWierzcholkow)
    {
        //znajdź sąsiadującą z grafem krawędź o najmniejszej wadze
        auto it = wierzcholkiWGrafie.begin();
        int e1, e2, eW;
        bool znaleziono = false;
        //znajdź pierwszą możliwą krawędź
        while (it != wierzcholkiWGrafie.end() && znaleziono == false)
        {
            if (!kolejkaSasiedztwa[*it].empty())
            {
                //jeśli krawędź prowadzi do wierzchołka który już jest w grafie, to ją usuń i szukaj dalej
                auto testIt = find(wierzcholkiWGrafie.begin(), wierzcholkiWGrafie.end(), kolejkaSasiedztwa[*it].top().second);
                if (testIt != wierzcholkiWGrafie.end())
                {
                    //cout << "Usunieto z kolejki: " << *it << ' ' << kolejkaSasiedztwa[*it].top().second << ' ' << kolejkaSasiedztwa[*it].top().first << endl;
                    kolejkaSasiedztwa[*it].pop();
                }
                else
                {
                    eW = kolejkaSasiedztwa[*it].top().first;
                    e1 = *it;
                    e2 = kolejkaSasiedztwa[*it].top().second;
                    it++;
                    znaleziono = true;
                }
            }
            else
                it++;
        }
        //sprawdź czy pozostałe wirzchołki nie mają krawędzi mniejszej wagi
        for (; it != wierzcholkiWGrafie.end(); it++)
        {
            if (!kolejkaSasiedztwa[*it].empty() && kolejkaSasiedztwa[*it].top().first < eW)
            {
                //jeśli krawędź prowadzi do wierzchołka który już jest w grafie, to ją usuń i szukaj dalej
                auto testIt = find(wierzcholkiWGrafie.begin(), wierzcholkiWGrafie.end(), kolejkaSasiedztwa[*it].top().second);
                if (testIt != wierzcholkiWGrafie.end())
                {
                    //cout << "Usunieto z kolejki: " << *it << ' ' << kolejkaSasiedztwa[*it].top().second << ' ' << kolejkaSasiedztwa[*it].top().first << endl;
                    kolejkaSasiedztwa[*it].pop();
                    it--;
                }
                else
                {
                    eW = kolejkaSasiedztwa[*it].top().first;
                    e1 = *it;
                    e2 = kolejkaSasiedztwa[*it].top().second;
                }
            }
        }
        //jeśli nie znaleziono odpowiedniej krawędzi to graf jest niespójny
        if (znaleziono == false) return false;
        //dodaj nową krawędź
        nowyGraf.dodajKrawedz(e1, e2, eW);
        nowyGraf.dodajKrawedz(e2, e1, eW);
        wierzcholkiWGrafie.push_back(e2);
        //cout << "Dodano: " << e1 << ' ' << e2 << ' ' << eW << endl;
        kolejkaSasiedztwa[e1].pop();
        //zaktualizuj kolejkę
        list<int>::iterator itSasiedztwa = listaSasiedztwa[e2].begin();
        list<int>::iterator itWag = listaWag[e2].begin();
        while (itSasiedztwa != listaSasiedztwa[e2].end())
        {
            {
                kolejkaSasiedztwa[e2].push(make_pair(*itWag, *itSasiedztwa));
            }
            advance(itSasiedztwa, 1);
            advance(itWag, 1);
        }
    }
    return nowyGraf;
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
            graf.dodajKrawedz(wierzcholek2 - 1, wierzcholek1 - 1, waga);
        }

        Graf nowyGraf = graf.MSTprim();
        int sumaWag = nowyGraf.sumaWag()/2;
        if (sumaWag == 0)
            cout << "brak";
        else
            cout << sumaWag;

        if (i < liczbaZestawow)
            cout << endl;
    }
}
