#include "table.h"
#include <iostream>
#include <random>

int main() {
  Table table(8, 2); // 8 people, 2 groups
  table.addPerson("alex", 0, {0, 0, 100, 25, 0, 0, 0, 100});
  table.addPerson("bob", 1, {0, 0, 0, 0, 50, 0, 0, 0});
  table.addPerson("claire", 2, {100, 0, 0, 0, 0, 100, 0, 0});
  table.addPerson("dave", 3, {25, 0, 0, 0, 0, -100, -50, 50});
  table.addPerson("ebeneezer", 4, {0, 50, 0, 0, 0, 0, 0, 0});
  table.addPerson("florence", 5, {0, 0, 100, -100, 0, 0, 0, 0});
  table.addPerson("gerald", 6, {0, 0, 0, -50, 0, 0, 0, 0});
  table.addPerson("hector", 7, {100, 0, 0, 50, 0, 0, 0, 0});

  if (!table.validate()) {
    std::cerr << "Error in table data" << std::endl;
    return 1;
  }

  // Shuffle the positions in the table to initialise
  table.shuffle();

  // Print the before, as well as the energy
  table.print();
  std::cout << "Energy before: " << table.energy() << std::endl << std::endl;

  // Initial temperature
  double temperature = 100;
  double step_amount = 0.1;

  // Initialise random
  std::random_device rd;
  std::uniform_int_distribution<int> people_dist(0, table.people() - 1);

  while (temperature > 0) {
    int32_t last_energy = table.energy();
    int32_t new_energy = last_energy;

    for (size_t i = 0; i < table.people(); i++) {
      // For each person, chose another random person to swap with
      uint8_t j = people_dist(rd);

      // can't swap with yourself
      if (i != j) {
        table.swap(i, j);
        new_energy = table.energy();

        // if this new state is worse, we can roll the dice
        if (new_energy < last_energy) {
          std::uniform_int_distribution<int> dist(0, temperature);

          // only if the energy change was within the a random range bounded by
          // current temperature
          if (last_energy - new_energy > dist(rd)) {
            table.swap(i, j);
          }
        }
      }
    }

    // Decay exponentially
    temperature -= (double)1 / pow(1.1, 100 - (temperature - step_amount));
    
    // Print temperature, useful to know
    std::cout << "\r temperature: " << temperature << ", energy: " << new_energy
              << "   " << std::flush;
  }

  std::cout << std::endl;
  table.print();
  std::cout << "Energy after: " << table.energy() << std::endl;

  return 0;
}