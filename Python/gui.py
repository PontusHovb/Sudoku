import pygame
import sys
import time

SIZE = 9
SCREEN_SIZE = 450
DURATION = 0.002
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
DARK_BLUE = (0, 0, 139)
LIGHT_BLUE = (173, 216, 230)

class GUI():
    def __init__(self, puzzle, title):
        pygame.init()
        self.font = pygame.font.Font(None, 36)
        self.block_size = SCREEN_SIZE // SIZE

        # Initialize window
        pygame.display.set_caption(title)
        self.screen = pygame.display.set_mode((SCREEN_SIZE, SCREEN_SIZE))
        self.screen.fill(WHITE)
        self.draw_grid(puzzle)
        pygame.display.flip()

    def run(self):
        start_time = time.time()
        running = True
        while running:
            for event in pygame.event.get():
                if event.type is pygame.QUIT:
                    running = False
            if time.time() - start_time > DURATION:
                running = False
            pygame.display.flip()

    def draw_grid(self, puzzle):
        for x in range(0, SCREEN_SIZE, self.block_size):
            line_width = 3 if x % (self.block_size * 3) == 0 else 1
            pygame.draw.line(self.screen, BLACK, (x, 0), (x, SCREEN_SIZE), line_width)
            pygame.draw.line(self.screen, BLACK, (0, x), (SCREEN_SIZE, x), line_width)

        for i in range(SIZE):
            for j in range(SIZE):
                if puzzle[i][j] != 0:  
                    text = self.font.render(str(puzzle[i][j]), True, BLACK)
                    text_x = j * self.block_size + (self.block_size - text.get_width()) // 2
                    text_y = i * self.block_size + (self.block_size - text.get_height()) // 2
                    self.screen.blit(text, (text_x, text_y))

    def show_number(self, row, col, number):
        self.screen.fill(LIGHT_BLUE, rect=[col * self.block_size + 2, row * self.block_size + 2, self.block_size  - 4, self.block_size - 4])
        text = self.font.render(str(number), True, DARK_BLUE)
        text_x = col * self.block_size + (self.block_size - text.get_width()) // 2
        text_y = row * self.block_size + (self.block_size - text.get_height()) // 2
        self.screen.blit(text, (text_x, text_y))
        self.run()
        self.screen.fill(WHITE, rect=[col * self.block_size + 2, row * self.block_size + 2, self.block_size  - 4, self.block_size - 4])

    def add_number(self, row, col, number):
        text = self.font.render(str(number), True, DARK_BLUE)
        text_x = col * self.block_size + (self.block_size - text.get_width()) // 2
        text_y = row * self.block_size + (self.block_size - text.get_height()) // 2
        self.screen.blit(text, (text_x, text_y))
       
def main():
    puzzle = [[0, 1, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 9, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0]]

    sudoku = GUI(puzzle, "Sudoku")
    sudoku.run()

    for row in range(3):
        for col in range(3):
            cell_solved = False
            guess = 1
            while not cell_solved:
                sudoku.show_number(row, col, guess)
            
                if guess == 9:
                    cell_solved = True

                guess += 1

    sudoku.run()

if __name__ == '__main__':
    main()