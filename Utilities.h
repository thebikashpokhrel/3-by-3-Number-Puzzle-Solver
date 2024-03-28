#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
using namespace std;
//Delay Function
void delay(int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

//Vector shuffling function
vector<int> shuffleVector(vector<int> v) {
    // Shuffle the vector
    random_device rd;
    mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
    for (int num : v) {
        cout << num << " ";
    }
    return v;
}

//Finding the number of inversions
int inv_num(std::vector<int>& puzzle) {
    int inv = 0;
    for (size_t i = 0; i < puzzle.size() - 1; i++) {
        for (size_t j = i + 1; j < puzzle.size(); j++) {
            if (puzzle[i] && puzzle[j] && puzzle[i] > puzzle[j]) {
                inv++;
            }
        }
    }
    return inv;
}

//Checking for the solvability of the puzzle
bool isSolvable(int inversions) {
    if (inversions % 2 == 0) return true;
    return false;
}
