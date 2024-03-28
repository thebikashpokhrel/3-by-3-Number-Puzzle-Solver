#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


//Function to check if the button is clicked or not
//Boundary = {xmin,xmax,ymin,ymax}
bool isBtnClicked(vector<int>boundary, Vector2i mousePos) {
    if (mousePos.x > boundary[0] && mousePos.x < boundary[1] && mousePos.y > boundary[2] && mousePos.y < boundary[3]) {
        return true;
    }
    else {
        return false;
    }
}
//Text Drawing
void drawText(RenderWindow& window, const string& text, const Font& font, int fontSize, const Vector2f position, const Color& color, Text::Style style = Text::Regular) {
    Text txt(text, font, fontSize);
    txt.setPosition(position);
    txt.setFillColor(color); // Set text color to black
    window.draw(txt);
}

//SFML Button
class sfButton {
public:
    sfButton(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor, const std::string& buttonText, const sf::Font& font, int characterSize)
        : position_(position), size_(size), idleColor_(idleColor), hoverColor_(hoverColor), activeColor_(activeColor) {
        rect_.setPosition(position_);
        rect_.setSize(size_);
        rect_.setFillColor(idleColor_);

        text_.setFont(font);
        text_.setString(buttonText);
        text_.setCharacterSize(characterSize);
        text_.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text_.getLocalBounds();
        text_.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text_.setPosition(position_ + size_ / 2.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(rect_);
        window.draw(text_);
    }

private:
    sf::Vector2f position_;
    sf::Vector2f size_;
    sf::Color idleColor_;
    sf::Color hoverColor_;
    sf::Color activeColor_;
    sf::RectangleShape rect_;
    sf::Text text_;
};


//SFML Progress Bar
void showProgress(RenderWindow& window, int progress, int posX) {
    RectangleShape progressBar(sf::Vector2f(progress, 40));
    progressBar.setPosition(posX, 680);
    Color c(56, 122, 223);
    progressBar.setFillColor(c);

    RectangleShape border(sf::Vector2f(300, 40));
    border.setPosition(posX, 680);
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(1);
    border.setOutlineColor(Color::Black);

    window.draw(border); // Draw the border first
    window.draw(progressBar);
}
