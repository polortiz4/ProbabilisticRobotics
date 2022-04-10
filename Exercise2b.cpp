#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>

enum Weather { Sunny, Cloudy, Rainy };

auto operator<<(std::ostream &out, const Weather value) -> std::ostream & {
  const char *s = 0;
#define PROCESS_VAL(p)                                                         \
  case (p):                                                                    \
    s = #p;                                                                    \
    break;
  switch (value) {
    PROCESS_VAL(Sunny);
    PROCESS_VAL(Cloudy);
    PROCESS_VAL(Rainy);
  }
#undef PROCESS_VAL

  return out << s;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: Exercise2b n_days";
    return EXIT_FAILURE;
  }

  const auto n_days = std::stoi(argv[1]);
  if (n_days < 1) {
    std::cerr << "At least one day must be passed.";
    return EXIT_FAILURE;
  }

  srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  auto dependency_array = std::array<std::array<double, 3>, 3>{};
  dependency_array[Weather::Sunny][Weather::Sunny] = 8;
  dependency_array[Weather::Sunny][Weather::Cloudy] = 2;
  dependency_array[Weather::Sunny][Weather::Rainy] = 0;

  dependency_array[Weather::Cloudy][Weather::Sunny] = 4;
  dependency_array[Weather::Cloudy][Weather::Cloudy] = 4;
  dependency_array[Weather::Cloudy][Weather::Rainy] = 2;

  dependency_array[Weather::Rainy][Weather::Sunny] = 2;
  dependency_array[Weather::Rainy][Weather::Cloudy] = 6;
  dependency_array[Weather::Rainy][Weather::Rainy] = 2;

  auto dist_generator = std::array<std::discrete_distribution<int>, 3>{};
  dist_generator[Weather::Sunny] =
      std::discrete_distribution<int>(dependency_array[Weather::Sunny].begin(),
                                      dependency_array[Weather::Sunny].end());

  dist_generator[Weather::Cloudy] =
      std::discrete_distribution<int>(dependency_array[Weather::Cloudy].begin(),
                                      dependency_array[Weather::Cloudy].end());

  dist_generator[Weather::Rainy] =
      std::discrete_distribution<int>(dependency_array[Weather::Rainy].begin(),
                                      dependency_array[Weather::Rainy].end());
  std::random_device rd;
  std::mt19937 gen(rd());

  //   const auto weather_today = Weather::Rainy;
  const auto weather_today = static_cast<Weather>(rand() % 3);

  const auto weather_tomorrow =
      static_cast<Weather>(dist_generator[weather_today](gen));

  auto weather_arr = std::vector<Weather>(static_cast<size_t>(n_days));
  weather_arr[0] = weather_today;
  std::cout << "Day 0: " << weather_arr[0] << std::endl;

  for (size_t i = 1; i < n_days; i++) {
    weather_arr[i] =
        static_cast<Weather>(dist_generator[weather_arr[i - 1]](gen));
    std::cout << "Day " << i << ": " << weather_arr[i] << std::endl;
  }

  return EXIT_SUCCESS;
}