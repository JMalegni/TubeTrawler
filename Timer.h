//
// Created by Jake on 12/3/2023.
//
#include <SFML/Graphics.hpp>
#pragma once

//this class comes from Joseph's minesweeper project from Programming Fundamentals 2
class Timer{
    sf::Clock gameTimer;
    bool isPaused;
    int curTime;

public:
    Timer() {
        isPaused = false;
        curTime = 0;
        gameTimer.restart();
    }

    void startTime() {
        if (isPaused) {
            gameTimer.restart();
        }
        isPaused = false;
    }

    void restartTime() {
        gameTimer.restart();
        curTime = 0;
        isPaused = false;
    }

    void pauseTime() {
        if (!isPaused) {
            curTime += gameTimer.getElapsedTime().asMicroseconds();
        }
        isPaused = true;
    }

    int getMicroSecondsElapsed() {
        if (!isPaused) {
            return curTime + gameTimer.getElapsedTime().asMicroseconds();
        }
        return curTime;
    }
};