#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using mojamapa_t = std::map<std::string, std::vector<double>>;
using mojafunkcja_t = std::function<std::double_t (std::vector<double>)>;

void wypisz(mojamapa_t mapa, mojafunkcja_t fun) {
    using namespace std;
    for (auto kv : mapa) {
        auto [k, v] = kv;
        cout << "klucz: " << k << "; wartosc " << fun(v) << endl;
    }
}

int main(int argc, char **argv) {
    using namespace std;
    map<string, mojafunkcja_t> formatery;
    formatery["sin"] = [](vector<double> x) { return sin(x[0]); };
    formatery["add"] = [](vector<double> x) { return x[0] + x[1]; };
    formatery["mod"] = [](vector<double> x) { return (int)x[0] % (int)x[1]; };

    try{
        vector<string> arguments(argv, argv + argc);
        auto fun = formatery.at(arguments.at(1));
        cout << fun(arguments) << endl;
    }catch (std::logic_error e) {
        cout << "\"sin x - liczy wartość funkcji sinus dla jednego argumentu.\\n add x y - sumuje dwie wartości\\n mod x y -oblicza wartość reszty z dzielenia\" " << endl;
        return 1;
    }
    return 0;
}