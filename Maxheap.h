//
// Created by blin1 on 12/2/2023.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>


//data of each element inside the heap
struct Heap_data {
    std::string word;
    int likes;
    int replies;
    Heap_data() {
        word = "";
        likes = 0;
        replies = 0;
    }
};

//the actual heap representation and functionality
class MaxHeap {
private:
    //using a vector instead of array to avoid manually resizing
    std::vector<Heap_data> heap;
public:
    void heapify_up() {
        //set the index for the newest data inserted, which is the child
        long long int child = heap.size() - 1;
        long long int parent = (child - 1) / 2;
        //since we called heapify_up everytime when we insert, we only need a while loop to control the max heap statuts
        while (parent >= 0 and heap[parent].likes < heap[child].likes) {
            std::swap(heap[parent], heap[child]);
            child = parent;
            parent = (child - 1) / 2;
        }
    }
    void heapify_down() {
        long long int left = 0;
        long long int right = 0;
        long long int parent = 0;
        long long int max = 0;
        while (left < heap.size() and right < heap.size()) {
            parent = max;
            left = parent * 2 + 1;
            right = parent * 2 + 2;
            if (heap[right].likes > heap[max].likes) {
                max = right;
            }
            if (heap[left].likes > heap[max].likes) {
                max = left;
            }
            std::swap(heap[max], heap[parent]);
            if (max == parent) {
                break;
            }
        }
    }
    //extract the most like word
    std::pair<std::string, int> extract () {
        std::pair<std::string, int> result = std::make_pair(heap[0].word, heap[0].likes);
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        heapify_down();
        return result;
    }
    void insert(std::string word, int likes, int replies) {
        Heap_data temp;
        temp.word = word;
        temp.likes = likes;
        temp.replies = replies;
        heap.push_back(temp);
        heapify_up();
    }
};