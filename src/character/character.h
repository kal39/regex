#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef enum CharacterType {
    CHAR_RANGE,
    CHAR_COMPLEMENT,
    CHAR_EMPTY,
} CharacterType;

typedef struct Character {
    CharacterType type;
    char start;
    char end;
} Character;

Character character(char c);
Character character_range(char start, char end);
Character character_complement(char start, char end);
Character character_empty();

bool character_is_empty(Character character);
bool character_in(Character character, char c);

void character_print(Character character, FILE* fp);