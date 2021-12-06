#include <iostream>
#include <list>

using namespace std;

class Graf
{
    int liczbaWierzcholkow;
    list<int>* listaSasiedztwa;
    bool czyCykleRoboczy(int wierzcholek, bool odwiedzone[], int rodzic);
public:
    Graf(int liczbaWierzcholkow);
    void dodajKrawedz(int wierzcholek1, int wierzcholek2);
    int skladoweSpojnosci();
    bool czyDwudzielny();
    bool czyCykle();
};

Graf::Graf(int liczbaWierzcholkow)
{
    this->liczbaWierzcholkow = liczbaWierzcholkow;
    listaSasiedztwa = new list<int>[liczbaWierzcholkow];
}

void Graf::dodajKrawedz(int wierzcholek1, int wierzcholek2)
{
    listaSasiedztwa[wierzcholek1].push_back(wierzcholek2);
}

int Graf::skladoweSpojnosci()
{
    int wierzcholekStartowy = 0;
    bool* odwiedzone = new bool[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        odwiedzone[i] = false;

    list<int> kolejka;

    int skladoweSpojnosci = 0;
    for (int lokalnyWierzcholekStartowy = 0; lokalnyWierzcholekStartowy < liczbaWierzcholkow; lokalnyWierzcholekStartowy++)
    {
        if (odwiedzone[lokalnyWierzcholekStartowy] == false)
        {
            skladoweSpojnosci++;
            odwiedzone[lokalnyWierzcholekStartowy] = true;
            kolejka.push_back(lokalnyWierzcholekStartowy);

            list<int>::iterator i;

            while (!kolejka.empty())
            {
                lokalnyWierzcholekStartowy = kolejka.front();
                //cout << lokalnyWierzcholekStartowy+1 << " ";
                kolejka.pop_front();

                for (i = listaSasiedztwa[lokalnyWierzcholekStartowy].begin(); i != listaSasiedztwa[lokalnyWierzcholekStartowy].end(); ++i)
                {
                    if (!odwiedzone[*i])
                    {
                        odwiedzone[*i] = true;
                        kolejka.push_back(*i);
                    }
                }
            }
        }
    }
    return skladoweSpojnosci;
}

bool Graf::czyDwudzielny()
{
    int wierzcholekStartowy = 0;
    bool* odwiedzone = new bool[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        odwiedzone[i] = false;

    int* pokolorowane = new int[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        pokolorowane[i] = -1;

    list<int> kolejka;

    for (int lokalnyWierzcholekStartowy = 0; lokalnyWierzcholekStartowy < liczbaWierzcholkow; lokalnyWierzcholekStartowy++)
    {
        if (odwiedzone[lokalnyWierzcholekStartowy] == false)
        {
            odwiedzone[lokalnyWierzcholekStartowy] = true;
            pokolorowane[lokalnyWierzcholekStartowy] = 1;
            kolejka.push_back(lokalnyWierzcholekStartowy);

            list<int>::iterator i;

            while (!kolejka.empty())
            {
                lokalnyWierzcholekStartowy = kolejka.front();
                //cout << lokalnyWierzcholekStartowy+1 << " ";
                kolejka.pop_front();

                for (i = listaSasiedztwa[lokalnyWierzcholekStartowy].begin(); i != listaSasiedztwa[lokalnyWierzcholekStartowy].end(); ++i)
                {
                    if (pokolorowane[*i] == -1)
                    {
                        odwiedzone[*i] = true;
                        pokolorowane[*i] = 1 - pokolorowane[lokalnyWierzcholekStartowy];
                        kolejka.push_back(*i);
                    }
                    else if (pokolorowane[*i] == pokolorowane[lokalnyWierzcholekStartowy])
                        return false;
                }
            }
        }
    }
    return true;
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
            int wierzcholek1, wierzcholek2;
            cin >> wierzcholek1;
            cin >> wierzcholek2;
            graf.dodajKrawedz(wierzcholek1-1, wierzcholek2-1);
            graf.dodajKrawedz(wierzcholek2-1, wierzcholek1-1);
        }

        cout << "Graf " << i << endl;
        
        if (graf.czyDwudzielny())
            cout << "Dwudzielny TAK" << endl;
        else
            cout << "Dwudzielny NIE" << endl;

        if (graf.skladoweSpojnosci() == 1)
            cout << "Spojny TAK (" << graf.skladoweSpojnosci() << ")" << endl;
        else
            cout << "Spojny NIE (" << graf.skladoweSpojnosci() << ")" << endl;

        if (graf.czyCykle())
            cout << "Cykle TAK" << endl;
        else
            cout << "Cykle NIE" << endl;

        if (!graf.czyCykle() && graf.skladoweSpojnosci() == 1)
            cout << "Drzewo TAK";
        else
            cout << "Drzewo NIE";

        if (i < liczbaZestawow) 
            cout << endl;
    }
}
