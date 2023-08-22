#include "table.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
#include <random>
#include <stdio.h>

Table::Table(uint8_t people, uint8_t groups)
    : m_people(people), m_groups(groups) {
  people_per_group = m_people / m_groups;
}
Table::~Table() {}

bool Table::addPerson(const char *name, uint8_t index,
                      std::vector<int8_t> associations) {
  if (index >= m_people || strlen(name) > max_name_size) {
    return false;
  }

  for (size_t i = 0; i < associations.size(); i++) {
    m_data.at(index).at(i) = associations.at(i);
  }

  strcpy(m_names.at(index).data(), name);

  return true;
}

bool Table::validate() {
  for (size_t i = 0; i < m_people; i++) {
    // Ensure indexes along the diagonal equal 0 (people can't have
    // relationships with themselves)
    if (m_data.at(i).at(i) != 0) {
      return false;
    }

    // Ensure indexes at the opposite side are matched, i.e alice and bob, bob
    // and alice
    for (size_t j = 0; j < m_people; j++) {
      if (m_data.at(i).at(j) != m_data.at(j).at(i)) {
        return false;
      }
    }
  }

  return true;
}

void Table::shuffle() {
  m_group.clear();

  // Create a buffer that indexes from 0-m_people
  // Then shuffle it
  std::vector<uint8_t> selected(m_people);
  std::iota(selected.begin(), selected.end(), 0);
  std::shuffle(selected.begin(), selected.end(),
               std::mt19937{std::random_device{}()});

  // For each group, just allocate chunks of the randomised vector and remove
  // from it after
  while (selected.size()) {
    std::array<uint8_t, max_people> group;
    for (size_t i = 0; i < people_per_group; i++) {
      group.at(i) = selected.at(i);
    }

    m_group.push_back(group);
    selected.erase(selected.begin(), selected.begin() + people_per_group);
  }
}

void Table::swap(uint8_t person_a, uint8_t person_b) {
  if (person_a >= m_people || person_b >= m_people) {
    return;
  }

  // For each group, find person a and swap with person b
  // Also find person b and swap for person a
  for (size_t group = 0; group < m_groups; group++) {
    for (uint8_t i = 0; i < people_per_group; i++) {
      if (m_group.at(group).at(i) == person_a) {
        m_group.at(group).at(i) = person_b;
      } else if (m_group.at(group).at(i) == person_b) {
        m_group.at(group).at(i) = person_a;
      }
    }
  }
}

char *Table::getPerson(uint8_t index) { return m_names.at(index).data(); }

void Table::print() {
  for (size_t i = 0; i < m_groups; i++) {
    if (i) {
      printf("| ");
    }
    printf("Group %-9zu", i);
  }
  printf("\n");

  for (size_t person = 0; person < people_per_group; person++) {

    for (size_t group = 0; group < m_groups; group++) {
      if (group) {
        printf("| ");
      }
      printf("%-15s", getPerson(m_group.at(group).at(person)));
    }
    printf("\n");
  }
}

int32_t Table::energy() {
  int32_t sum = 0;

  // calculate the energy for each group
  for (size_t group = 0; group < m_group.size(); group++) {
    sum += energy(group);
  }

  return sum;
}

int32_t Table::energy(uint8_t group) {
  // validate parameters
  if (group >= m_group.size()) {
    return 0;
  }

  int32_t sum = 0;

  // get the relationship value for each pair in group (2^n matches)
  for (size_t i = 0; i < people_per_group; i++) {
    for (size_t j = 0; j < people_per_group; j++) {
      uint8_t person_a = m_group.at(group).at(i);
      uint8_t person_b = m_group.at(group).at(j);
      sum += m_data.at(person_a).at(person_b);
    }
  }
  sum >>= 2; // divide by 2 because we're searching everyone twice
  // inefficient, i know

  // include neighbour relationships
  for (size_t i = 0; i < people_per_group - 1; i++) {
    uint8_t person_a = m_group.at(group).at(i);
    uint8_t person_b = m_group.at(group).at(i + 1);
    sum += m_data.at(person_a).at(person_b);
  }

  return sum;
}