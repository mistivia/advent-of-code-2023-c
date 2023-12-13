#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "str.h"

typedef struct {
    int r,g,b;
} set_t;

typedef struct {
    int length;
    set_t sets[64];
} game_t;

game_t games[100];

void parse_set(const char *str, set_t *set) {
    char **strs = str_split(str, ',');
    set->r = 0;
    set->g = 0;
    set->b = 0;
    while (*strs != NULL) {
        char **infos = str_split(str_strip(*strs), ' ');
        if (strcmp(infos[1], "red") == 0) {
            set->r = strtol(infos[0], NULL, 10);
        } else if (strcmp(infos[1], "blue") == 0) {
            set->b = strtol(infos[0], NULL, 10);
        } else if (strcmp(infos[1], "green") == 0) {
            set->g = strtol(infos[0], NULL, 10);
        }
        strs++;
    }
}

void parse_game(const char *line, game_t *game) {
    char **strs = str_split(line, ':');
    char **sets_str = str_split(strs[1], ';');
    int i = 0;
    while (*sets_str != NULL) {
        set_t s;
        parse_set(str_strip(*sets_str), &s);
        game->sets[i] = s;

        sets_str++;
        i++;
    }
    game->length = i;
}

int is_possible(game_t *game) {
    for (int i = 0; i < game->length; i++) {
        if (game->sets[i].r <= 12
                && game->sets[i].g <= 13
                && game->sets[i].b <= 14) {
            continue;
        }
        return 0;
    }
    return 1;
}

int main() {
    FILE *fp = fopen("./input", "r");
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        char *line = fgetline(fp);
        parse_game(line, &games[i]);
    }
    for (int i = 0; i < 100; i++) {
        if (is_possible(&games[i])) {
            sum += i + 1;
        }
    }
    printf("%d\n", sum);    
    return 0;
}
