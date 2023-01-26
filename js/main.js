// canvas
const canvasWidth = 1_000;
const canvasHeight = 500;

const canvasBackground = "#ddd";

// cell
const cellWidth = 25;
const cellHeight = 25;

// border
const borderColor = "#333";
const borderWidth = 1.0 // 0.1 < x < 20

// snake
const snakeColor = "#000";
const snakeStartSize = 3;
const snakeSpeed = 5; // moves per second

// apple
const appleColor = "#00ff00";

// game over screen
const gameOverText = "60px serif"; // syntax same with css font

const canvas = document.getElementById("game");
const game = canvas.getContext('2d');
const scoreBoard = document.getElementById("score");

// set size of canvas, ofcourse it is not adaptive
game.canvas.width = canvasWidth;
game.canvas.height = canvasHeight;

// set background
canvas.style.background = canvasBackground;

var score = 0;


/* DRAW BORDERS */
function drawBorder(){
  game.fillStyle = borderColor;
  game.lineWidth = borderWidth;

  game.beginPath();

  // draw row lines
  for (var i = cellWidth; i < canvasWidth; i += cellWidth){
    game.moveTo(i, 0);
    game.lineTo(i, canvasHeight);
  }

  // draw column lines
  for (var i = cellHeight; i < canvasHeight; i += cellHeight){
    game.moveTo(0, i);
    game.lineTo(canvasWidth, i);
  }

  game.stroke();
}


/* SNAKE */
function drawSnake(){
  game.fillStyle = snakeColor;

  snake.forEach(e => {
    game.fillRect(e[0] * cellWidth, e[1] * cellHeight, cellWidth, cellHeight);
  });
}

function moveSnake() {
  snake.pop();
  snake.unshift([snake[0][0], snake[0][1]]);

  if (direction == "right"){++snake[0][0];}
  else if (direction == "up"){--snake[0][1];}
  else if (direction == "left"){--snake[0][0];}
  else {++snake[0][1];}
}



var snake = new Array();

// generate snake's head
snake.push([Math.floor(canvasWidth / cellWidth / 2), Math.floor(canvasHeight / cellHeight / 2)]);

// generate other parts
for (var i = 1; i < snakeStartSize; i++){
  snake.push([snake[i - 1][0] - 1, snake[i - 1][1]]);
}

/* APPLE */
function generateApple(){
  apple = getRandomCell();

  if (snake.includes(apple)) {
    return generateApple();
  }

  return apple;
}

function drawApple(){
  game.fillStyle = appleColor;
  game.fillRect(apple[0] * cellWidth, apple[1] * cellHeight, cellWidth, cellHeight);
}

var apple = generateApple();


/* GAME */
var direction = "right";
var score = 0;

function getRandomCell(){
  return [Math.floor(Math.random() * (canvasWidth / cellWidth)), Math.floor(Math.random() * (canvasHeight / cellHeight))]; // 0 indexed x, y coordinates
}

// user control
document.addEventListener("keyup", (e) => {
  if (e.code === "ArrowRight" && direction !== "left"){
    direction = "right";
  }
  else if (e.code === "ArrowUp" && direction !== "down"){
    direction = "up";
  }
  else if (e.code === "ArrowLeft" && direction !== "right"){
    direction = "left";
  }
  else if (e.code === "ArrowDown" && direction !== "up"){
    direction = "down";
  }
});

// check for collision
function collision(){
  // if apple eaten
  if (snake[0][0] == apple[0] && snake[0][1] == apple[1]){
    apple = generateApple();

    ++score;
    scoreBoard.innerText = score;

    snake.push([snake.at(-1)[0], snake.at(-1)[1]]);
  }

  // teleport to opposite side, if snake hits wall
  if (snake[0][0] >= canvasWidth / cellWidth){snake[0][0] = 0;} //left wall
  else if (snake[0][1] < 0){snake[0][1] = canvasHeight / cellHeight;} // up wall
  else if (snake[0][0] < 0){snake[0][0] = canvasWidth / cellWidth;} // right wall
  else if (snake[0][1] >= canvasHeight / cellHeight){snake[0][1] = 0;} // bottom wall

  // if snake eats itself -> game over
  for(var i = 1; i < snake.length; i++){
    if(snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) {
      return true;
    }
  }
}

//action
game.textAlign = "center";
game.font = "30px sans-serif";
game.fillText("Press any key to start the game!", canvasWidth / 2, canvasHeight / 2);

var isGameStarted = false;
document.addEventListener("keyup", () => {
  if (!isGameStarted){
    const interval = setInterval(() => {
      action();
    }, 1000 / snakeSpeed);

    isGameStarted = true;
  }
})

function action(){
  var isGameOver = collision();

  game.clearRect(0, 0, canvasWidth, canvasHeight);

  if (isGameOver){
    game.textAlign = "center";
    game.font = gameOverText;
    game.fillText("Game Over", canvasWidth / 2, canvasHeight / 2);

    game.textAlign = "center";
    game.font = "15px sans-serif";
    game.fillText("Reload to play again", canvasWidth / 2, canvasHeight / 2 + 30);

    cleatInterval(interval);
  }

  drawApple();

  drawSnake();
  moveSnake();

  drawBorder();
}
