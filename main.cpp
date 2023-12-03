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
#include "Timer.h"
#include <filesystem>
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

    //simple decorative bar
    sf::RectangleShape navBar(sf::Vector2f(800.f, 15.f));
    navBar.setFillColor(sf::Color(255, 0, 0));  // YouTube red
    navBar.setPosition(0.f, 50.f);

    // Set up the logo
    sf::Texture logoTexture;
    logoTexture.loadFromFile("../tubetrawlerlogo.png");
    sf::Sprite logo(logoTexture);
    logo.setScale(0.14f, 0.14f);
    logo.setPosition(675.f, 10.f);

    //input prompt text element
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

    //user input text element
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

    //Simple rectangle shape for where the user inputs text
    sf::RectangleShape textBox(sf::Vector2f(windowWidth/1.5f - 18, 28));
    textBox.setOrigin(userRect.left + userRect.width/2.0f, userRect.top + userRect.height/2.0f);
    textBox.setPosition(sf::Vector2f(windowWidth/3.0f - 118, windowHeight/2.0f - 48));
    textBox.setFillColor(sf::Color(210,210,210));

    //this text element shows up when the input is not a url
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
    // Joseph modified the text on it to read "Red-Black Tree" and "Max Heap"
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

    //this clock will be used to have a blinking cursor
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

                //catches mouse clicks outside the window
                if (curPos.x < 0 || curPos.x > windowWidth || curPos.y < 0 || curPos.y > windowHeight) {
                    cout << "clicked out of bounds" << endl;
                }

                //when there is a click within the coordinates of the max heap button
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
                    else { //set the invalid input flag to show the invalid input message
                        validInput = false;
                        videoURL.clear();
                        textBox.setFillColor(sf::Color(255, 127, 127));

                    }

                }
                //same deal as the Max Heap button
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
                //resetting the valid input flag so that the invalid message text goes away
                validInput = true;

                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!videoURL.empty()) {
                        videoURL.pop_back();
                    }
                }

            }

            // allows ctrl V for pasting urls
            else if (event.key.control && event.key.code == sf::Keyboard::V) {
                videoURL = sf::Clipboard::getString();
            }
        }

        // logic for blinking effect on the cursor comes from user Mortal at https://en.sfml-dev.org/forums/index.php?topic=26927.0
        //setting up other necessary variables for the blinking cursor
        static sf::Time effectTime;
        static bool showCursor;

        effectTime += clock.restart();

        if (effectTime >= sf::seconds(0.5f))
        {
            showCursor = !showCursor;
            effectTime = sf::Time::Zero;
        }

        if (showCursor){
            user.setString(videoURL + '|');
        }
        else {
            user.setString(videoURL + ' ');
        }

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
    //reading the first line which is garbage for the data structures
    getline(file, line);
    while (getline(file, line)){
        //reads the comment, then the number of likes, then the number of replies for each line
        std::istringstream splitter(line);
        std::string word = "";
        getline(splitter, word, ',');
        std::string likes = "";
        getline(splitter, likes, ',');
        std::string replies = "";
        getline(splitter, replies, ',');
        //gets choice from the user selected button
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
void resultWindow(string& systemCommand, const string& dataStructChoice){
    float windowWidth = 800;
    float windowHeight = 600;

    sf::RenderWindow resultWindow(sf::VideoMode(windowWidth, windowHeight), "TubeTrawler Results");

    // font used with permission - https://www.fontsquirrel.com/fonts/open-sans
    sf::Font font;
    font.loadFromFile("../OpenSans-Regular.ttf");

    //simple text element that shows uo while the csv is being loaded
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

    //displaying loading screen so the user isn't just left without anything to look at while the csv is being filled
    resultWindow.clear(sf::Color(40,40,40));
    resultWindow.draw(loadMsgText);
    resultWindow.display();

    // this line opens terminal and calls the python script with the chosen video ID
    system(systemCommand.c_str());

    // Displays "Done!" for a short while before displaying results
    for (int i = 0; i < 250; i++) {
        loadMsg = "Done!";
        loadMsgText.setString(loadMsg);
        loadMsgRect = loadMsgText.getLocalBounds();
        loadMsgText.setOrigin(loadMsgRect.left + loadMsgRect.width/2.0f, loadMsgRect.top + loadMsgRect.height/2.0f);
        resultWindow.clear(sf::Color(40, 40, 40));
        resultWindow.draw(loadMsgText);
        resultWindow.display();
    }

    // reading the first line of the csv to get the general video statistics (title, views, likes, comments)
    std::fstream file("../test.csv");

    //as a last line of defense in my input validation, if the python script fails to access the api, then this will display
    if (filesystem::is_empty("../test.csv")){
        string errMsg = "Not A Valid Youtube Link";

        for (int i = 0; i < 2000; i++) {
            loadMsgText.setString(errMsg);
            loadMsgRect = loadMsgText.getLocalBounds();
            loadMsgText.setOrigin(loadMsgRect.left + loadMsgRect.width/2.0f, loadMsgRect.top + loadMsgRect.height/2.0f);
            resultWindow.clear(sf::Color(40, 40, 40));
            resultWindow.draw(loadMsgText);
            resultWindow.display();
        }
        return;
    }

    std::string vidStats = "";
    getline(file, vidStats);

    //setting up the text element for the video title
    string vidTitle = "Video Title: " + vidStats.substr(0,vidStats.find_first_of(','));
    sf::Text vidTitleText;
    vidTitleText.setFont(font);
    vidTitleText.setString(vidTitle);
    vidTitleText.setCharacterSize(22);
    vidTitleText.setStyle(sf::Text::Bold);
    vidTitleText.setFillColor(sf::Color::White);
    sf::FloatRect vidTitleRect = vidTitleText.getLocalBounds();
    vidTitleText.setOrigin(vidTitleRect.left + vidTitleRect.width/2.0f, vidTitleRect.top + vidTitleRect.height/2.0f);
    vidTitleText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 150));

    //this parses through the rest of the first line of the csv, inserting words to explain each value
    vidStats = vidStats.substr(vidStats.find_first_of(',') + 1);
    string vidNumStats = "Views: " + vidStats.substr(0,vidStats.find_first_of(','));
    vidStats = vidStats.substr(vidStats.find_first_of(',') + 1);
    vidNumStats += "; Likes: " + vidStats.substr(0,vidStats.find_first_of(','));
    vidStats = vidStats.substr(vidStats.find_first_of(',') + 1);
    vidNumStats += "; Comments: " + vidStats.substr(0);

    //
    sf::Text vidStatsText;
    vidStatsText.setFont(font);
    vidStatsText.setString(vidNumStats);
    vidStatsText.setCharacterSize(20);
    vidStatsText.setStyle(sf::Text::Bold);
    vidStatsText.setFillColor(sf::Color::White);
    sf::FloatRect vidStatsRect = vidStatsText.getLocalBounds();
    vidStatsText.setOrigin(vidStatsRect.left + vidStatsRect.width/2.0f, vidStatsRect.top + vidStatsRect.height/2.0f);
    vidStatsText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f-125));

    //instantiaing the max heap, red black tree, and starting the timer
    RBT myRBT;
    MaxHeap myHeap;
    Timer dataStructTimer;

    dataStructTimer.startTime();

    readfile(myRBT, myHeap, dataStructChoice);
    vector<string> results;
    //making sure that if there is an issue, the program doesn't display old results that the user didn't want
    results.clear();

    if (dataStructChoice == "RBT")
    {
        results = myRBT.mostLiked(myRBT.getRoot());
    }
    else if (dataStructChoice == "MaxHeap")
    {
        results = myHeap.extract();
    }
    else
    {
        return;//neither choice
    }

    string structSpeed;
    if (dataStructChoice == "RBT") {
        structSpeed = "It took the Red-Black Tree " + to_string(dataStructTimer.getMicroSecondsElapsed()) +
                      " Microseconds to Generate";
    }
    else if (dataStructChoice == "MaxHeap"){
        structSpeed = "It took the Max Heap " + to_string(dataStructTimer.getMicroSecondsElapsed()) +
                      " Microseconds to Generate";
    }

    dataStructTimer.pauseTime();

    cout << '"' << results.at(0) << '"' << endl << "Likes: " << results.at(1) << endl << "Replies: " << results.at(2) << endl;

    // set up the results window in a very similar manner to the welcome window
    sf::Text header("TubeTrawler Results", font, 24);
    header.setFillColor(sf::Color::White);
    header.setPosition(10.f, 10.f);

    sf::RectangleShape navBar(sf::Vector2f(800.f, 15.f));
    navBar.setFillColor(sf::Color(255, 0, 0));
    navBar.setPosition(0.f, 50.f);

    sf::Texture logoTexture;
    logoTexture.loadFromFile("../tubetrawlerlogo.png");
    sf::Sprite logo(logoTexture);
    logo.setScale(0.14f, 0.14f);  // Adjust the scale as needed
    logo.setPosition(675.f, 10.f);

    //text element for the actual most liked comment from the video
    string MLComment = "\"" + results.at(0) + "\"";
    sf::Text MLCommentText;
    MLCommentText.setFont(font);
    MLCommentText.setString(MLComment);
    MLCommentText.setCharacterSize(20);
    MLCommentText.setStyle(sf::Text::Bold);
    MLCommentText.setFillColor(sf::Color::White);
    //Solution for text wrapping, Credit for following for loop to Laurent Gomila - SFML developer
    //https://en.sfml-dev.org/forums/index.php?topic=20346.0
    for (int i = 0; i < MLCommentText.getString().getSize(); ++i)
    {
        if (MLCommentText.findCharacterPos(i).x > windowWidth - 100)
        {
            string str = MLCommentText.getString();
            str.insert(i, "\n");
            MLCommentText.setString(str);
        }
    }
    sf::FloatRect MLCommentRect = MLCommentText.getLocalBounds();
    MLCommentText.setOrigin(MLCommentRect.left + MLCommentRect.width/2.0f, MLCommentRect.top + MLCommentRect.height/2.0f);
    MLCommentText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f));

    //text element for displaying the likes and number of replies for the most liked comment
    string commentStats = "Is the Most Liked Comment, With " + results.at(1) + " Likes, and " + results.at(2) + " Replies";
    sf::Text commentStatsText;
    commentStatsText.setFont(font);
    commentStatsText.setString(commentStats);
    commentStatsText.setCharacterSize(20);
    commentStatsText.setStyle(sf::Text::Bold);
    commentStatsText.setFillColor(sf::Color::White);
    sf::FloatRect commentStatsRect = commentStatsText.getLocalBounds();
    commentStatsText.setOrigin(commentStatsRect.left + commentStatsRect.width/2.0f, commentStatsRect.top + commentStatsRect.height/2.0f);
    commentStatsText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f + MLCommentText.getGlobalBounds().height+20));

    //text element for displaying the time it takes for the data structure to be filled and to extract the most liked comment
    sf::Text structSpeedText;
    structSpeedText.setFont(font);
    structSpeedText.setString(structSpeed);
    structSpeedText.setCharacterSize(20);
    structSpeedText.setStyle(sf::Text::Bold);
    structSpeedText.setFillColor(sf::Color::White);
    sf::FloatRect structSpeedRect = structSpeedText.getLocalBounds();
    structSpeedText.setOrigin(structSpeedRect.left + structSpeedRect.width/2.0f, structSpeedRect.top + structSpeedRect.height/2.0f);
    structSpeedText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f + MLCommentText.getGlobalBounds().height+60));

    while (resultWindow.isOpen()) {
        sf::Event event;

        while (resultWindow.pollEvent(event)) {

            sf::Cursor cursor;
            if (cursor.loadFromSystem(sf::Cursor::Hand)) {
                resultWindow.setMouseCursor(cursor);
            }

            if (event.type == sf::Event::Closed) {
                resultWindow.close();
            }
        }

        resultWindow.clear(sf::Color(40,40,40));
        resultWindow.draw(navBar);
        resultWindow.draw(header);
        resultWindow.draw(logo);
        resultWindow.draw(vidTitleText);
        resultWindow.draw(vidStatsText);
        resultWindow.draw(MLCommentText);
        resultWindow.draw(commentStatsText);
        resultWindow.draw(structSpeedText);

        resultWindow.display();
    }
};

int main() {
    string choice = ""; //choice will be either "RBT" or "MaxHeap"
    string ID = welcomeWindow(choice);
    string command = "";

    cout << "---" << ID << "---" << endl;

    // if the entered input is correct, set up the command to run the python script
    // need to have python as path variable
    if (!ID.empty()) {
        //Figured out how to pass a variable to system() with help from user Skurmedel's solution at
        // https://stackoverflow.com/questions/4907805/using-variables-in-system-function-c
        command = "cd .. && python main.py ";
        command += ID;
    }

    //call the results window which runs the terminal command to run the python script and calls the datastructure classes/functions
    if (!command.empty()) {
        resultWindow(command, choice);
    }

    /* FIXME: There is an error that pops up in very large videos(at about half a million comments) Best guess right now is that its an issue with how I am continuously getting comments with the python script*/

    return 0;

}
