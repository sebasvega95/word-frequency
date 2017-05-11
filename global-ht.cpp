#include <getopt.h>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "lib/file.hpp"

std::unordered_map<std::string, int> freq;
std::mutex mtx;

void count_words(std::string &text) {
  std::stringstream words(text);
  std::string word;
  while (words >> word) {
    std::transform(word.begin(), word.end(), word.begin(), tolower);
    word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
    mtx.lock();
    freq[word]++;
    mtx.unlock();
  }
}

void usage(char *program_name) {
  int n = 1;
  std::string opts[] = {"-j, --threads"};
  std::string description[] = {"Number of threads to use"};

  std::cout << "Usage: " << program_name << " [options ...] text" << std::endl;
  std::cout << std::endl;
  std::cout << "Options" << std::endl;
  for (int i = 0; i < n; i++) {
    std::cout << "  " << opts[i] << ": " << description[i] << std::endl;
  }

  exit(EXIT_FAILURE);
}

void run_program(std::string filename, size_t num_threads) {
  std::vector<std::thread> threads(num_threads);
  std::vector<std::string> text = file::read_word_chunks(filename, num_threads);

  for (size_t i = 0; i < num_threads; i++) {
    threads[i] = std::thread(count_words, std::ref(text[i]));
  }

  for (auto &th : threads) {
    th.join();
  }

  for (auto &elem : freq) {
    std::string word = elem.first;
    int count = elem.second;
    std::cout << word << ": " << count << std::endl;
  }
}

int main(int argc, char *argv[]) {
  int opt, opt_index = 0;
  static struct option options[] = {{"threads", required_argument, 0, 'j'},
                                    {0, 0, 0, 0}};

  size_t num_threads = 1;
  while ((opt = getopt_long(argc, argv, "j:", options, &opt_index)) != -1) {
    switch (opt) {
      case 'j':
        num_threads = atoi(optarg);
        break;
      default:
        usage(argv[0]);
        break;
    }
  }

  if (argc - optind != 1) {
    std::cout << "Error: You must provide a text" << std::endl << std::endl;
    usage(argv[0]);
  }

  std::string filename = argv[optind];
  if (!file::exists(filename)) {
    std::cout << "Error: Text file not found" << std::endl;
    exit(EXIT_FAILURE);
  }

  run_program(filename, num_threads);
  return 0;
}
