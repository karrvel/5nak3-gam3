#!/bin/python3
import pygame as pg
from pygame.locals import K_RIGHT, K_UP, K_LEFT, K_DOWN

from random import randint
from config import *

pg.init()
screen = pg.display.set_mode(SIZE)

pg.display.set_caption(caption)

score = 0
state = 1 # -1 = quit, 0 = play, 1 = wait, 2 = game over
clock = pg.time.Clock()


def show_score():    
    font = pg.font.SysFont(ff, score_fsize)
    text = font.render(str(score), True, score_color)
    text_rect = text.get_rect()
    text_rect.center = (SIZE[0] // 2, score_fsize // 2 - 5)
    screen.blit(text, text_rect)

def draw_borders():
    # draw columns
    for i in range(0, SIZE[0] // c_size[0]):
        pg.draw.line(screen, b_color, [i * c_size[0], 0], [i * c_size[0], SIZE[1]], b_w)

    # draw rows
    for i in range(0, SIZE[1] // c_size[1]):
        pg.draw.line(screen, b_color, [0, i * c_size[1]], [SIZE[0], i * c_size[1]], b_w)

def start_screen():
    font = pg.font.SysFont(ff, start_fsize)
    text = font.render("Press SPACE to start the game", True, start_color)
    text_rect = text.get_rect()
    text_rect.center = (SIZE[0] // 2, SIZE[1] // 2)
    screen.blit(text, text_rect)

def game_over_screen():
    screen.fill(background)

    font = pg.font.SysFont(ff, game_over_fsize)
    text = font.render("Game Over", True, game_over_color)
    text_rect = text.get_rect()
    text_rect.center = (SIZE[0] // 2, SIZE[1] // 2)
    screen.blit(text, text_rect)

    font = pg.font.SysFont(ff, 20)
    text = font.render("Press SPACE to play again", True, game_over_color)
    text_rect = text.get_rect()
    text_rect.center = (SIZE[0] // 2, SIZE[1] // 2 + 50)
    screen.blit(text, text_rect)


class Snake():
    def __init__(self):
        self.color = s_color
        self.speed = s_speed
        self.start_size = s_start_size

        self.generate()

    def generate(self):
        score = 0
        self.direction = "right"
        self.snake = []
        self.snake.append([SIZE[0] / c_size[0] // 2, SIZE[1] / c_size[1] // 2])
        for i in range(1, self.start_size):
            self.snake.append([self.snake[i - 1][0] - 1, self.snake[i - 1][1]])

    def user_control(self, event):
        try: event.key
        except: return

        if event.key == K_RIGHT and self.direction != "left":
            self.direction = "right"
        elif event.key == K_UP and self.direction != "down":
            self.direction = "up"
        elif event.key == K_LEFT and self.direction != "right":
            self.direction = "left"
        elif event.key == K_DOWN and self.direction != "up":
            self.direction = "down"

    def add_tail(self):
        global score
        score += 1
        self.snake.append(self.snake[-1].copy())

    def update(self):
        # if it hits itself -> game_over
        for i in range(1, len(self.snake)):
            if self.snake[0][0] == self.snake[i][0] and self.snake[0][1] == self.snake[i][1]:
                return "game over"

        # if it reaches to sides -> teleports to other side
        if self.snake[0][0] < 0: self.snake[0][0] = SIZE[0] // c_size[0] - 1     # left -> right
        elif self.snake[0][0] >= SIZE[0] // c_size[0]: self.snake[0][0] = 0      # right -> left
        elif self.snake[0][1] < 0: self.snake[0][1] = SIZE[1] // c_size[1] - 1   # up -> down
        elif self.snake[0][1] >= SIZE[1] // c_size[1]: self.snake[0][1] = 0      # down -> up

        # else make a move
        else:
            del self.snake[-1]
            self.snake.insert(0, self.snake[0].copy())

            if self.direction == "right": self.snake[0][0] += 1
            elif self.direction == "up": self.snake[0][1] -= 1
            elif self.direction == "left": self.snake[0][0] -= 1
            else: self.snake[0][1] += 1

    def draw(self):
        for x, y in self.snake:
            rect = pg.Rect(x * c_size[0], y * c_size[1], *c_size)
            pg.draw.rect(screen, self.color, rect)


class Apple():
    def __init__(self):
        self.color = a_color
        self.apple = self.generate()

    def generate(self):
        apple = [randint(0, SIZE[0] // c_size[0] - 1), randint(0, SIZE[1] // c_size[1] - 1)]

        for x, y in snake.snake:
            if apple[0] == x and apple[1] == y:
                return self.generate()
        return apple

    def update(self):
        # if apple is eaten
        if self.apple[0] == snake.snake[0][0] and self.apple[1] == snake.snake[0][1]:
            snake.add_tail()
            self.apple = self.generate()

    def draw(self):
        rect = pg.Rect(self.apple[0] * c_size[0], self.apple[1] * c_size[1], *c_size)
        pg.draw.rect(screen, self.color, rect)


snake = Snake()
apple = Apple()

while state != -1:
    clock.tick(s_speed)
    screen.fill(background)

    for event in pg.event.get():
        if event.type == pg.QUIT:
            state = -1
            break
        
        try:
            if event.key == pg.K_SPACE and state == 1:
                state = 0
            elif event.key == pg.K_SPACE and state == 2:
                state = 1
                snake.generate()
            else:
                snake.user_control(event)
        except: pass


    if state == -1: break
    elif state == 1:
        start_screen()
        pg.display.flip()
        continue
    elif state == 2:
        game_over_screen()
        show_score()
        pg.display.flip()
        continue

    apple.update()
    apple.draw()

    if snake.update() == "game over":
        state = 2
    snake.draw()

    draw_borders()
    show_score()
    pg.display.flip()


pg.quit()
