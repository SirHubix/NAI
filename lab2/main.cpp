#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>
#include <algorithm>


using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());

domain_t
hill_climbing(const std::function<double(domain_t)> &f, domain_t start_point, std::function<std::vector<domain_t>()> get_close_points, int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points();
        auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(), [f](auto a, auto b){return f(a) > f(b);});
        if (f(best_neighbour) < f(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

int main(int argc, char **argv) {
    std::map<std::string, std::function<double(std::vector<double>)>> hillFormatery;
    std::vector<std::string> argumenty(argv, argc + argv);

    hillFormatery["booth"] = [](domain_t x) {return pow((x[0]+ 2*x[1] - 7),2) + pow((2*x[0]+x[1]-5),2);};
    hillFormatery["matyas"] = [](domain_t x) { return 0.26 * (x[0] * x[0] + x[1] * x[1]) - (0.48 * x[0] * x[1]); };
    hillFormatery["beale"] = [](domain_t x) {return pow((1.5-x[0]+x[0]*x[1]),2) + pow(2.25-x[0]+x[0]*x[1]*x[1],2) + pow(2.265-x[0]+x[0]*x[1]*x[1]*x[1],2);};

    auto get_random_point = [=]() -> domain_t {
        std::uniform_real_distribution<double>distr(std::stod(argumenty.at(2)), std::stod(argumenty.at(3)));
        return {distr(mt_generator), distr(mt_generator)};
    };
    auto get_close_points_random = [=]() -> std::vector<domain_t> {
        std::uniform_real_distribution<double>distr(std::stod(argumenty.at(2)),std::stod(argumenty.at(3)));
        return {{distr(mt_generator), distr(mt_generator)}};
    };

    try {
        auto best = hill_climbing(hillFormatery.at(argumenty.at(1)), get_random_point(), get_close_points_random, 100000);
        std::cout << "best x = " << best[0] << std::endl;
        std::cout << "best y = " << best[1] << std::endl;



    }
    catch (std::out_of_range oof) {
        std::cout << "Podano zla funkcje!" << std::endl;
    }
    return 0;
}