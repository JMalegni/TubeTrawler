//
// Created by blin1 on 12/2/2023.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
        //check to see if the left and right indexes are still valid
        while (left < heap.size() and right < heap.size()){
            //set the parent index
            parent = max;
            left = parent * 2 + 1;
            right = parent * 2 + 2;
            if (heap[right].likes > heap[max].likes) {
                max = right;
            }
            if (heap[left].likes > heap[max].likes) {
                max = left;
            }
            //swap the max index with the parent, could be left, right, or no changce
            std::swap(heap[max], heap[parent]);
            if (max == parent) {
                break;
            }
        }
    }
    //extract the most like word
    std::vector<std::string> extract () {
        std::vector<std::string> result;
        result.push_back(heap[0].word);
        result.push_back(std::to_string(heap[0].likes));
        result.push_back(std::to_string(heap[0].replies));
        //make the last item become first one
        heap[0] = heap[heap.size() - 1];
        //remove the last item
        heap.pop_back();
        heapify_down();
        return result;
    }
    void insert(std::string word, int likes, int replies) {
        //capture a Heap_data with temp
        Heap_data temp;
        temp.word = word;
        temp.likes = likes;
        temp.replies = replies;
        heap.push_back(temp);
        heapify_up();
    }
};