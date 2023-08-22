#pragma once
#include <stdint.h>
#include <array>
#include <vector>

class Table
{
private:
    static constexpr uint8_t max_people = 16;
    static constexpr uint8_t max_name_size = 15;
    uint8_t m_people;
    uint8_t m_groups;
    size_t people_per_group;
    
    std::array<std::array<char, 32>, max_people> m_names;
    std::array<std::array<int8_t, max_people>, max_people> m_data; // 2x2 array grid of associations

    std::vector<std::array<uint8_t, max_people>> m_group;

public:
    Table(uint8_t people, uint8_t groups);
    ~Table();
    uint8_t people() { return m_people; }
    uint8_t groups() { return m_groups; }
    bool addPerson(const char* name, uint8_t index, std::vector<int8_t> associations);
    char* getPerson(uint8_t index);
    void shuffle();
    void swap(uint8_t, uint8_t);
    int32_t energy(uint8_t group);
    int32_t energy();
    bool validate();
    void print();
};

