#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

//SFML download and integration tutorial - https://dev.to/danielmelendezz/how-to-get-smfl-to-work-on-windows-using-clion-2bef

//WelcomeWindow Function mostly recycled from Joseph's Minesweeper project from COP3503
string welcomeWindow() {
    float windowWidth = 1500;
    float windowHeight = 1500;

    sf::RenderWindow welWindow(sf::VideoMode(windowWidth, windowHeight), "Welcome!");

    // font used with permission - https://www.fontsquirrel.com/fonts/open-sans
    sf::Font font;
    font.loadFromFile("../OpenSans-Regular.ttf");

    string title = "Welcome to TubeTrawler";
    sf::Text message;
    message.setFont(font);
    message.setString(title);
    message.setCharacterSize(24);
    message.setFillColor(sf::Color::White);
    message.setStyle(sf::Text::Bold);
    sf::FloatRect titleRect = message.getLocalBounds();
    message.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top + titleRect.height/2.0f);
    message.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 150));

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
    user.setPosition(sf::Vector2f(windowWidth/3.0f+18, windowHeight/2.0f - 45));

    sf::RectangleShape textBox(sf::Vector2f(windowWidth/2.75f - 36, 28));
    textBox.setPosition(sf::Vector2f(windowWidth/3.0f, windowHeight/2.0f - 48));
    textBox.setFillColor(sf::Color(210,210,210));


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

            else if (event.type == sf::Event::TextEntered) {
                if (std::isprint(event.text.unicode) && videoURL.size() < 43)
                    videoURL += event.text.unicode;

            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!videoURL.empty()) {
                        videoURL.pop_back();
                    }
                }

                if (event.key.code == sf::Keyboard::Return) {
                    cout << videoURL << endl;
                    welWindow.close();
                    return videoURL.substr(videoURL.size() - 11, 11);
                    //TODO:: use substr to just get ID then call next function here
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

        welWindow.clear(sf::Color(0,0,255));
        welWindow.draw(textBox);
        welWindow.draw(user);
        welWindow.draw(message);
        welWindow.draw(inputPromptText);
        welWindow.display();

    }
    return "";
}



int main() {
    //FIXME:: Needs a lot more input validation but I am very eepy at this moment
    string ID= welcomeWindow();

    cout << "---" << ID << "---" << endl;

    // need to have python as path variable
    //42nrFpATLow is a YouTube video id that is temporary, I will make it so that we can get it as input from the user
    if (!ID.empty()) {
        string command = "cd .. && python main.py ";
        command += ID;
        system(command.c_str());
    }


    return 0;

}
