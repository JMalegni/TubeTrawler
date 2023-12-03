#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <regex>
#include "Maxheap.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <queue>
#include "RBT.h"
using namespace std;

//SFML download and integration tutorial - https://dev.to/danielmelendezz/how-to-get-smfl-to-work-on-windows-using-clion-2bef

//WelcomeWindow Function mostly recycled from Joseph's Minesweeper project from COP3503
string welcomeWindow(string& dataStruct) {
    float windowWidth = 800;
    float windowHeight = 600;
    bool validInput = true;

    sf::RenderWindow welWindow(sf::VideoMode(windowWidth, windowHeight), "TubeTrawler");

    // font used with permission - https://www.fontsquirrel.com/fonts/open-sans
    sf::Font font;
    font.loadFromFile("../OpenSans-Regular.ttf");

    sf::Text header("TubeTrawler", font, 24);
    header.setFillColor(sf::Color::White);
    header.setPosition(10.f, 10.f);

    sf::RectangleShape navBar(sf::Vector2f(800.f, 15.f));
    navBar.setFillColor(sf::Color(255, 0, 0));  // YouTube red
    navBar.setPosition(0.f, 50.f);

    // Set up the logo
    sf::Texture logoTexture;
    logoTexture.loadFromFile("../tubetrawlerlogo.png");

    sf::Sprite logo(logoTexture);
    logo.setScale(0.14f, 0.14f);  // Adjust the scale as needed
    logo.setPosition(675.f, 10.f);

    string inputPrompt = "Enter A YouTube Video URL";
    sf::Text inputPromptText;
    inputPromptText.setFont(font);
    inputPromptText.setString(inputPrompt);
    inputPromptText.setCharacterSize(20);
    inputPromptText.setFillColor(sf::Color::White);
    inputPromptText.setStyle(sf::Text::Bold);
    sf::FloatRect inputPromptRect = inputPromptText.getLocalBounds();
    inputPromptText.setOrigin(inputPromptRect.left + inputPromptRect.width/2.0f, inputPromptRect.top + inputPromptRect.height/2.0f);
    inputPromptText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 75));

    string videoURL = "";
    sf::Text user;
    user.setFont(font);
    user.setString(videoURL);
    user.setCharacterSize(18);
    user.setStyle(sf::Text::Bold);
    user.setFillColor(sf::Color::Black);
    sf::FloatRect userRect = user.getLocalBounds();
    user.setOrigin(userRect.left + userRect.width/2.0f, userRect.top + userRect.height/2.0f);
    user.setPosition(sf::Vector2f(windowWidth/3.0f - 100, windowHeight/2.0f - 45));

    sf::RectangleShape textBox(sf::Vector2f(windowWidth/1.5f - 18, 28));
    textBox.setPosition(sf::Vector2f(windowWidth/3.0f - 118, windowHeight/2.0f - 48));
    textBox.setFillColor(sf::Color(210,210,210));

    string errorMsg = "Input is invalid. Please enter a valid Youtube video URL";
    sf::Text invalidMsg;
    invalidMsg.setFont(font);
    invalidMsg.setString(errorMsg);
    invalidMsg.setCharacterSize(18);
    invalidMsg.setStyle(sf::Text::Bold);
    invalidMsg.setFillColor(sf::Color(255, 127, 127));
    sf::FloatRect invalidMsgRect = invalidMsg.getLocalBounds();
    invalidMsg.setOrigin(invalidMsgRect.left + invalidMsgRect.width/2.0f, invalidMsgRect.top + invalidMsgRect.height/2.0f);
    invalidMsg.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f + 25));

    //button background png is used with permission from "https://www.flaticon.com/free-icons/enter" Enter icons created by Freepik
    sf::Texture RBTTexture;
    RBTTexture.loadFromFile("../RBTButton.png");
    sf::Sprite RBTButton;
    RBTButton.setTexture(RBTTexture);

    sf::FloatRect RBTRect = RBTButton.getLocalBounds();
    RBTButton.setOrigin(invalidMsgRect.left + RBTRect.width/2.0f, RBTRect.top + RBTRect.height/2.0f);
    RBTButton.setPosition(sf::Vector2f(windowWidth/2.0f + 100, windowHeight/2.0f + 110));
    RBTButton.setScale(.25f, .25f);

    sf::Texture MaxHeapTexture;
    MaxHeapTexture.loadFromFile("../MaxHeapButton.png");
    sf::Sprite MaxHeapButton;
    MaxHeapButton.setTexture(MaxHeapTexture);

    sf::FloatRect MaxHeapRect = MaxHeapButton.getLocalBounds();
    MaxHeapButton.setOrigin(invalidMsgRect.left + MaxHeapRect.width/2.0f, MaxHeapRect.top + MaxHeapRect.height/2.0f);
    MaxHeapButton.setPosition(sf::Vector2f(windowWidth/2.0f - 100, windowHeight/2.0f + 110));
    MaxHeapButton.setScale(.25f, .25f);

    sf::Clock clock;

    while (welWindow.isOpen()) {
        sf::Event event;
        while (welWindow.pollEvent(event)) {

            sf::Cursor cursor;
            if (cursor.loadFromSystem(sf::Cursor::Hand)) {
                welWindow.setMouseCursor(cursor);
            }

            if (event.type == sf::Event::Closed) {
                welWindow.close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                sf::Vector2i curPos = sf::Mouse::getPosition(welWindow);
                cout << curPos.x << " , " << curPos.y << endl;

                if (curPos.x < 0 || curPos.x > windowWidth || curPos.y < 0 || curPos.y > windowHeight) {
                    cout << "clicked out of bounds" << endl;
                }
                else if (curPos.y <= 455 && curPos.y >= 365 && curPos.x >= 240 && curPos.x <= 360) {
                    dataStruct = "MaxHeap";
                    cout << videoURL << endl;

                    string vidId;
                    //this try-catch stops error with input size < 11 chars
                    try {
                        vidId = videoURL.substr(32, 11);
                    }
                    catch (out_of_range&){
                        videoURL.clear();
                    }

                    //regex to only allow 11 characters, upper and lower case letters, and the '-' symbol
                    regex pattern("[a-zA-Z0-9\\-\\_]{11}");

                    if (regex_match(vidId, pattern) && videoURL.substr(0,32) == "https://www.youtube.com/watch?v=") {
                        welWindow.close();
                        return vidId;
                    }
                    else {
                        validInput = false;
                        videoURL.clear();
                        textBox.setFillColor(sf::Color(255, 127, 127));

                    }

                }
                else if (curPos.y <= 455 && curPos.y >= 365 && curPos.x >= 440 && curPos.x <= 560) {
                    dataStruct = "RBT";
                    cout << videoURL << endl;

                    string vidId;
                    //this try-catch stops error with input size < 11 chars
                    try {
                        vidId = videoURL.substr(32, 11);
                    }
                    catch (out_of_range&){
                        videoURL.clear();
                    }

                    //regex to only allow 11 characters, upper and lower case letters, and the '-' symbol
                    regex pattern("[a-zA-Z0-9\\-\\_]{11}");

                    if (regex_match(vidId, pattern) && videoURL.substr(0,32) == "https://www.youtube.com/watch?v=") {
                        welWindow.close();
                        return vidId;
                    }
                    else {
                        validInput = false;
                        videoURL.clear();
                        textBox.setFillColor(sf::Color(255, 127, 127));

                    }

                }
            }

            else if (event.type == sf::Event::TextEntered) {
                if (std::isprint(event.text.unicode) && videoURL.size() < 43)
                    videoURL += event.text.unicode;
            }

            else if (event.type == sf::Event::KeyPressed) {
                textBox.setFillColor(sf::Color(210,210,210));
                validInput = true;

                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!videoURL.empty()) {
                        videoURL.pop_back();
                    }
                }

            }

            else if (event.key.control && event.key.code == sf::Keyboard::V) {
                videoURL = sf::Clipboard::getString();
            }
        }

        static sf::Time text_effect_time;
        static bool show_cursor;

        text_effect_time += clock.restart();

        if (text_effect_time >= sf::seconds(0.5f))
        {
            show_cursor = !show_cursor;
            text_effect_time = sf::Time::Zero;
        }

        user.setString(videoURL + (show_cursor ? '_' : ' '));

        welWindow.clear(sf::Color(40,40,40)); // YouTube "almost black"
        welWindow.draw(textBox);
        welWindow.draw(user);
        welWindow.draw(navBar);
        welWindow.draw(header);
        welWindow.draw(logo);
        welWindow.draw(inputPromptText);
        welWindow.draw(RBTButton);
        welWindow.draw(MaxHeapButton);
        if (!validInput){welWindow.draw(invalidMsg);}
        welWindow.display();

    }
    return "";
}


void readfile(RBT &myRBT, MaxHeap &myHeap, const string& choice) {
    std::fstream file("../test.csv");
    std::string line = "";
    getline(file, line);
    while (getline(file, line)){
        std::istringstream splitter(line);
        std::string word = "";
        getline(splitter, word, ',');
        std::string likes = "";
        getline(splitter, likes, ',');
        std::string replies = "";
        getline(splitter, replies, ',');
        if (choice == "MaxHeap")
        {
            myHeap.insert(word, stoi(likes), stoi(replies));
        }
        else if (choice == "RBT")
        {
            myRBT.insert(word, stoi(replies), stoi(likes));
        }
        else
        {
            //neither choice
        }
    }
};
void resultWindow(string systemCommand){
    float windowWidth = 800;
    float windowHeight = 600;

    sf::RenderWindow resultWindow(sf::VideoMode(windowWidth, windowHeight), "TubeTrawler Results");

    // font used with permission - https://www.fontsquirrel.com/fonts/open-sans
    sf::Font font;
    font.loadFromFile("../OpenSans-Regular.ttf");

    string loadMsg = "Loading Comments";
    sf::Text loadMsgText;
    loadMsgText.setFont(font);
    loadMsgText.setString(loadMsg);
    loadMsgText.setCharacterSize(35);
    loadMsgText.setFillColor(sf::Color::White);
    loadMsgText.setStyle(sf::Text::Bold);
    sf::FloatRect loadMsgRect = loadMsgText.getLocalBounds();
    loadMsgText.setOrigin(loadMsgRect.left + loadMsgRect.width/2.0f, loadMsgRect.top + loadMsgRect.height/2.0f);
    loadMsgText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f));

    resultWindow.clear(sf::Color(40,40,40));
    resultWindow.draw(loadMsgText);
    resultWindow.display();

    system(systemCommand.c_str());

    for (int i = 0; i < 200; i++) {
        loadMsg = "Done!";
        loadMsgText.setString(loadMsg);
        loadMsgRect = loadMsgText.getLocalBounds();
        loadMsgText.setOrigin(loadMsgRect.left + loadMsgRect.width/2.0f, loadMsgRect.top + loadMsgRect.height/2.0f);
        resultWindow.clear(sf::Color(40, 40, 40));
        resultWindow.draw(loadMsgText);
        resultWindow.display();
    }


};

int main() {
    string choice = ""; //choice will be either "RBT" or "MaxHeap"
    string ID = welcomeWindow(choice);

    cout << "---" << ID << "---" << endl;

    // need to have python as path variable
    if (!ID.empty()) {
        //Figured out how to pass a variable to system() with help from user Skurmedel's solution at
        // https://stackoverflow.com/questions/4907805/using-variables-in-system-function-c
        string command = "cd .. && python main.py ";
        command += ID;
        resultWindow(command);
    }

    cout << "Finished making csv" << endl;

    RBT myRBT;
    MaxHeap myHeap;

    readfile(myRBT, myHeap, choice);
    vector<string> results;
    results.clear();

    if (choice == "RBT")
    {
        results = myRBT.mostLiked(myRBT.getRoot());
        cout << "By using a Red-Black Tree, we find the most liked comment to be: " << endl;
    }
    else if (choice == "MaxHeap")
    {
        results = myHeap.extract();
        cout << "By using a Max Heap, we find the most liked comment to be: " << endl;
    }
    else
    {
        return 0;//neither choice
    }

    cout << '"' << results.at(0) << '"' << endl << "Likes: " << results.at(1) << endl << "Replies: " << results.at(2) << endl;

    /* FIXME: There is an error that pops up in very large videos(at about half a million comments) Best guess right now is that its an issue with how I am continuously getting comments with the python script*/

    return 0;

}
