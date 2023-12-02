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

int power(game_t *game) {
    int maxr = 0;
    int maxg = 0;
    int maxb = 0;
    for (int i = 0; i < game->length; i++) {
        if (game->sets[i].r > maxr) maxr = game->sets[i].r;
        if (game->sets[i].g > maxg) maxg = game->sets[i].g;
        if (game->sets[i].b > maxb) maxb = game->sets[i].b;
    }
    return maxr * maxg * maxb;
}

void unit_test() {
    set_t s;
    
    parse_set("1 green, 2 blue, 3 red", &s);
    assert(s.r == 3 && s.b == 2 && s.g == 1);
    
    parse_set(" 9 green, 5 blue", &s);
    assert(s.g == 9 && s.b == 5 && s.r == 0);
    
    game_t game;
    parse_game("Game 26: 11 red, 9 blue; 3 blue, 3 red, 3 green; 10 blue, 3 green, 4 red; 1 green, 4 blue, 9 red; 5 green, 1 red, 7 blue; 1 red, 3 blue, 3 green", &game);
    assert(game.length == 6);
    assert(game.sets[5].b == 3);

    parse_game("Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red", &game);
    assert(power(&game) == 630);
}


int main() {
    // unit_test();
    FILE *fp = fopen("./input", "r");
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        char *line = fgetline(fp);
        parse_game(line, &games[i]);
    }
    for (int i = 0; i < 100; i++) {
        sum += power(&games[i]);
    }
    printf("%d\n", sum);    
    return 0;
}
