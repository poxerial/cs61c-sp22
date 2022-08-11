#include "state.h"
#include "snake_utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for
 * you). */
static char get_board_at(game_state_t *state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for
 * you). */
static void set_board_at(game_state_t *state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state() {
  game_state_t *state = malloc(sizeof(game_state_t));
  state->x_size = 14;
  state->y_size = 10;
  state->board = malloc(state->y_size * sizeof(char *));
  for (int i = 0; i < state->y_size; i++) {
    state->board[i] = malloc((state->x_size + 1) * sizeof(char));
    if (i == 0 || i == state->y_size - 1)
      memset(state->board[i], '#', state->x_size);
    else if (i > 0 && i < state->y_size - 1) {
      memset(state->board[i], ' ', state->x_size);
      state->board[i][0] = '#';
      state->board[i][state->x_size - 1] = '#';
    }
    state->board[i][state->x_size] = '\0';
  }

  state->num_snakes = 1;
  state->snakes = malloc(state->num_snakes * sizeof(snake_t));
  state->snakes[0].tail_x = 4;
  state->snakes[0].tail_y = 4;
  state->snakes[0].head_x = 5;
  state->snakes[0].head_y = 4;
  state->snakes[0].live = true;
  state->board[state->snakes[0].tail_y][state->snakes[0].tail_x] = 'd';
  state->board[state->snakes[0].head_y][state->snakes[0].head_x] = '>';

  state->board[2][9] = '*';

  return state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  free(state->snakes);
  for (int i = 0; i < state->y_size; i++)
    free(state->board[i]);
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->y_size; i++)
    fprintf(fp, "%s\n", state->board[i]);
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) { return strchr("wasd", c); }

static bool is_snake(char c) { return strchr("wasd^<>vx", c); }

static char body_to_tail(char c) {
  if (!strchr("^<>v", c))
    return '?';
  switch (c) {
  case '^':
    return 'w';
  case '<':
    return 'a';
  case '>':
    return 'd';
  case 'v':
    return 's';
  }
  assert(false);
  return '?';
}

static int incr_x(char c) {
  if (strchr(">d", c))
    return 1;
  if (strchr("<a", c))
    return -1;
  return 0;
}

static int incr_y(char c) {
  if (strchr("vs", c))
    return 1;
  if (strchr("^w", c))
    return -1;
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum) {
  assert(snum < state->num_snakes);
  const snake_t s = state->snakes[snum];
  if (s.live == false)
    return 'x';

  char head = get_board_at(state, s.head_x, s.head_y);
  if (incr_x(head) != 0)
    return get_board_at(state, s.head_x + incr_x(head), s.head_y);
  else if (incr_y(head) != 0)
    return get_board_at(state, s.head_x, s.head_y + incr_y(head));
  return '?';
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum) {
  snake_t *s = state->snakes + snum;
  if (!s->live)
    return;
  char head = get_board_at(state, s->head_x, s->head_y);
  int x = incr_x(head);
  if (x)
    s->head_x += x;
  int y = incr_y(head);
  if (y)
    s->head_y += y;
  if (x || y)
    state->board[s->head_y][s->head_x] = head;
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum) {
  snake_t *s = state->snakes + snum;
  char tail = get_board_at(state, s->tail_x, s->tail_y);
  state->board[s->tail_y][s->tail_x] = ' ';
  int x = incr_x(tail);
  if (x)
    s->tail_x += x;
  int y = incr_y(tail);
  if (y)
    s->tail_y += y;
  if (x || y)
    state->board[s->tail_y][s->tail_x] =
        body_to_tail(get_board_at(state, s->tail_x, s->tail_y));
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  for (int i = 0; i < state->num_snakes; i++) {
    snake_t *snake = state->snakes + i;
    char next = next_square(state, i);
    if (is_snake(next) || next == '#') {
      snake->live = false;
      state->board[snake->head_y][snake->head_x] = 'x';
      continue;
    } else {
      update_head(state, i);
    }
    if (next == '*') {
      add_food(state);
    } else {
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename) {
  game_state_t *state = malloc(sizeof(game_state_t));
  FILE *file = fopen(filename, "r");
  char buffer[512];
  fscanf(file, "%[^\n]\n", buffer);
  state->x_size = strlen(buffer);
  char *row_buffer[512];
  row_buffer[0] = calloc(state->x_size + 1, sizeof(char));
  strcpy(row_buffer[0], buffer);

  state->y_size = 1;
  for (; !feof(file); state->y_size++) {
    if (state->y_size >= 512) {
      printf("\nfilename: %s\n", filename);
      assert(false);
    }
    row_buffer[state->y_size] = calloc(state->x_size + 1, sizeof(char));
    fscanf(file, "%[^\n]\n", row_buffer[state->y_size]);
  }
  state->board = malloc(state->y_size * sizeof(char *));
  memcpy(state->board, row_buffer, state->y_size * sizeof(char *));

  return state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum) {
  int x = state->snakes[snum].tail_x;
  int y = state->snakes[snum].tail_y;
  int delta_x = 0;
  int delta_y = 0;
  do {
    x += delta_x;
    y += delta_y;
    delta_x = incr_x(get_board_at(state, x, y));
    delta_y = incr_y(get_board_at(state, x, y));
  } while (is_snake(get_board_at(state, x + delta_x, y + delta_y)));
  state->snakes[snum].head_x = x;
  state->snakes[snum].head_y = y;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  state->num_snakes = 0;
  state->snakes = malloc(64 * sizeof(snake_t));
  for (int y = 0; y < state->y_size; y++)
    for (int x = 0; x < state->x_size; x++) {
      if (strchr("wasd", get_board_at(state, x, y))) {
        state->snakes[state->num_snakes].live = true;
        state->snakes[state->num_snakes].tail_x = x;
        state->snakes[state->num_snakes].tail_y = y;
        find_head(state, state->num_snakes);
        state->num_snakes++;
      }
    }
  state->snakes = realloc(state->snakes, state->num_snakes * sizeof(snake_t));
  return state;
}
