// Copyright 2021 Maria Melnikova dandelion.m@mail.ru
#include "test_cache.hpp"

MyExperiment::MyExperiment(const size_t &N)
    : _times()
      , _number_of_elements(N) {
  _mas = new int[_number_of_elements];

  //  std::random_device rd;
  //  std::mt19937 mersenne(rd());
  std::mt19937 generator(clock());
  for (size_t i = 0; i < _number_of_elements; ++i)
    _mas[i] = generator();

  for (size_t i = 0; i < _number_of_elements; i += 16)
    _indexes_for_warming.push_back(i);

  for (size_t i = 0; i < _number_of_elements; i++)
    _indexes_for_buffer.push_back(i);
}

size_t MyExperiment::get_number_of_elements() const {
  return  _number_of_elements;
}

ExperimentTimes MyExperiment::get_time() const {
  return _times;
}

int MyExperiment::get_element(int a) {
  return a;
}

void MyExperiment::warming_cache() {
  for (const size_t &i : _indexes_for_warming) {
    get_element(_mas[i]);
  }
}

auto MyExperiment::loop_mas() {
  auto start_time = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < 1000; ++i) {
    for (const size_t &j : _indexes_for_buffer)
      get_element(_mas[j]);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_ns = std::chrono::
      duration_cast<std::chrono::milliseconds>(end_time - start_time);
  return elapsed_ns.count();
}

void MyExperiment::make_straight_experiment() {
  warming_cache();
  _times.time_straight = loop_mas();
}

void MyExperiment::make_reverse_experiment() {
  std::reverse(_indexes_for_warming.begin(), _indexes_for_warming.end());
  warming_cache();
  std::reverse(_indexes_for_buffer.begin(), _indexes_for_buffer.end());
  _times.time_reverse = loop_mas();
}

void MyExperiment::make_random_experiment() {
  std::mt19937 g(clock());
  std::shuffle(_indexes_for_warming.begin(), _indexes_for_warming.end(), g);
  warming_cache();
  std::shuffle(_indexes_for_buffer.begin(), _indexes_for_buffer.end(), g);
  _times.time_random = loop_mas();
}

MyExperiment::~MyExperiment() {
  delete[] _mas;
}
