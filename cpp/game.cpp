#include <QPainter>
#include <QTime>
#include <QKeyEvent>

#include <random>
#include "game.h"

Snake::Snake(QWidget *parent) : QWidget(parent){
  setFixedSize(WIDTH, HEIGHT);
  setWindowTitle("5nak3 gam3");
  setStyleSheet(static_cast<QString>("background-color: ") + background);
  setAutoFillBackground(true);

  score = 0;
  direction = "right";
  state = "start";

  apple.assign({-1, -1});

  generateSnake();
  generateApple();
  timerId = startTimer(1000 / snakeSpeed);
}

void Snake::paintEvent(QPaintEvent *e){  
  Q_UNUSED(e);

  if (state == "start"){
    startScreen();
  }
  else if (state == "game_over"){
    showScore();
    gameOverScreen();
  }
  else {
  drawSnake();
  drawApple();
  drawBorders();
  showScore();
  }
}

void Snake::timerEvent(QTimerEvent *e){
  Q_UNUSED(e);
  if (state == "game"){
    updateSnake();
    updateApple();
  }

  repaint();
}

/* SCREENS */
void Snake::startScreen(){
  QPen pen;
  pen.setColor(QColor(startColor));

  QPainter painter(this);
  painter.setPen(pen);
  painter.setRenderHint(QPainter::Antialiasing);

  QFont font("Sans-Serif", startSize);
  painter.setFont(font);
  
  painter.drawText(QRectF(0, 0, WIDTH, HEIGHT), Qt::AlignCenter, "Press SPACE to play!");
}

void Snake::gameOverScreen(){
  QPen pen;
  pen.setColor(QColor(gameOverColor));

  QPainter painter(this);
  painter.setPen(pen);
  painter.setRenderHint(QPainter::Antialiasing);

  QFont font1("Sans-Serif", gameOverSize1);
  painter.setFont(font1);

  painter.drawText(QRectF(0, 0, WIDTH, HEIGHT - 100), Qt::AlignCenter, "Game Over");

  QFont font2("Sans-Serif", gameOverSize2);
  painter.setFont(font2);

  painter.drawText(QRectF(0, 0, WIDTH, HEIGHT + 100), Qt::AlignCenter, "Press SPACE to play again!");
}

void Snake::showScore(){
  QPen pen;
  pen.setColor(QColor(scoreColor));

  QPainter painter(this);
  painter.setPen(pen);
  painter.setRenderHint(QPainter::Antialiasing);

  QFont font("Sans-Serif", scoreSize);
  painter.setFont(font);
  
  painter.drawText(QRectF(0, 0, WIDTH, scoreSize + 20), Qt::AlignCenter, QString::number(score));
 
}

/* BORDER */
void Snake::drawBorders(){
  QPen pen;
  pen.setWidth(borderWidth);
  pen.setColor(QColor(borderColor));

  QPainter painter(this);
  painter.setPen(pen);

  // draw rows
  for (size_t i = 0; i < HEIGHT; i += cellHeight){
    painter.drawLine(0, i, WIDTH, i);
  }
  // draw columns
  for (size_t i = 0; i < WIDTH; i += cellWidth){
    painter.drawLine(i, 0, i, HEIGHT);
  }
}


/* SNAKE */
void Snake::generateSnake(){
  std::vector<long long int> v {WIDTH / cellWidth / 2, HEIGHT / cellHeight / 2};
  snake.push_back(v);

  for(size_t i = 1; i < snakeStartSize; i++){
    std::vector<long long int> v {snake[i - 1][0] - 1, snake[i - 1][1]};
    snake.push_back(v);
  }
}

void Snake::addTail(){
  snake.push_back(snake.back());
}

void Snake::updateSnake(){
  // if snake hits itself -> game over
  for(size_t i = 1; i < snake.size(); i++){
    if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]){
      state = "game_over";
      return;
    }
  }

  snake.pop_back();
  snake.insert(snake.begin(), snake[0]);
  // if snake reaches sides -> teleport to other side
  if (snake[0][0] >= static_cast<long long int>(WIDTH / cellWidth)) snake[0][0] = 0;
  else if (snake[0][0] < 0) snake[0][0] = WIDTH / cellWidth - 1;
  
  else if (snake[0][1] >= static_cast<long long int>(HEIGHT / cellHeight)) snake[0][1] = 0;
  else if (snake[0][1] < 0) snake[0][1] = HEIGHT / cellHeight - 1;

  // else make a move
  else {
    if (direction == "right") ++snake[0][0];
    else if (direction == "up") --snake[0][1];
    else if (direction == "left") --snake[0][0];
    else ++snake[0][1];
  }
}

void Snake::keyPressEvent(QKeyEvent *e){
  if (e->key() == Qt::Key_Space && state == "start"){
    state = "game";
  }

  if (e->key() == Qt::Key_Space && state == "game_over"){
    snake = {};
    generateSnake();
    generateApple();
    score = 0;
    direction = "right";
    state = "start";
  }

  if (e->key() == Qt::Key_Right && direction != "left") direction = "right";
  else if (e->key() == Qt::Key_Up && direction != "down") direction = "up";
  else if (e->key() == Qt::Key_Left && direction != "right") direction = "left";
  else if (e->key() == Qt::Key_Down && direction != "up") direction = "down";

  QWidget::keyPressEvent(e);
}

void Snake::drawSnake(){
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::SolidPattern);
  painter.setBrush(QColor(snakeColor));

  for(std::vector<long long int> e : snake){
    painter.drawRect(e[0] * cellWidth, e[1] * cellHeight, cellWidth, cellHeight);
  }
}

/* APPLE */
void Snake::generateApple(){
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  
  // x
  std::uniform_int_distribution<> distrX(0, WIDTH / cellWidth - 1); // define the range
  apple[0] = distrX(gen);

  // y
  std::uniform_int_distribution<> distrY(0, HEIGHT / cellHeight - 1); // define the range
  apple[1] = distrY(gen);

  for(std::vector<long long int> e : snake)
    if(apple[0] == e[0] && apple[1] == e[1])
      generateApple();
}

void Snake::updateApple(){
  // if apple is eaten
  if(apple[0] == snake[0][0] && apple[1] == snake[0][1]){
    score++;
    addTail();

    generateApple();
  }
}

void Snake::drawApple(){
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::SolidPattern);
  painter.setBrush(QColor(appleColor));

  painter.drawRect(apple[0] * cellWidth, apple[1] * cellHeight, cellWidth, cellHeight);
}
