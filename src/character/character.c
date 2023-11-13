#include "character.h"

Character character(char c) {
    return (Character){CHAR_RANGE, c, c};
}

Character character_range(char start, char end) {
    return (Character){CHAR_RANGE, start, end};
}

Character character_complement(char start, char end) {
    return (Character){CHAR_COMPLEMENT, start, end};
}

Character character_empty() {
    return (Character){CHAR_EMPTY, 0, 0};
}

bool character_is_empty(Character character) {
    return character.type == CHAR_EMPTY;
}

bool character_in(Character character, char c) {
    switch (character.type) {
        case CHAR_RANGE: return c >= character.start && c <= character.end;
        case CHAR_COMPLEMENT: return c < character.start || c > character.end;
        case CHAR_EMPTY: return c == 0;
        default: return false;
    }
}

void character_print(Character character, FILE* fp) {
    switch (character.type) {
        case CHAR_RANGE:
            if (character.start == character.end)
                fprintf(fp, "%c", character.start);
            else fprintf(fp, "%c-%c", character.start, character.end);
            break;
        case CHAR_COMPLEMENT:
            if (character.start == character.end)
                fprintf(fp, "^%c", character.start);
            else fprintf(fp, "^%c-%c", character.start, character.end);
            break;
        case CHAR_EMPTY: fprintf(fp, " "); break;
    }
}