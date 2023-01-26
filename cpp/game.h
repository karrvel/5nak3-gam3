#pragma once

#include <QWidget>
#include <vector>

/* CONFIG */
// screen
const size_t WIDTH = 1000;
const size_t HEIGHT = 500;
const QString background = "#ccc";

const QString startColor = "#000";
const size_t startSize = 20;

const QString gameOverColor = "#000";
const size_t gameOverSize1 = 60;
const size_t gameOverSize2 = 10;

// score
const QString scoreColor = "#444";
const size_t scoreSize = 30;

// cell
const size_t cellWidth = 25;
const size_t cellHeight = 25;

// border
const size_t borderWidth = 1;
const QString borderColor = "#333";

// snake
const size_t snakeStartSize = 3;
const size_t snakeSpeed = 5;
const QString snakeColor = "#000";

// apple
const QString appleColor = "#004400";


class Snake : public QWidget{
  public:
    Snake(QWidget *parent = nullptr);

  protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

  private:
    int timerId;
    size_t score;
    QString direction;
    QString state; // start, game, game_over
    std::vector<std::vector<long long int>> snake;
    std::vector<long long int> apple;

    void showScore();
    void startScreen();
    void gameOverScreen();

    void drawBorders();

    void generateSnake();
    void addTail();
    void updateSnake();
    void drawSnake();
 
    void generateApple();
    void updateApple();
    void drawApple();
};
