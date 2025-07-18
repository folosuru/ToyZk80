#include <stdarg.h>
#include <string.h>

#include "assembler.h"

int resolveLabel(struct unresolved_label* find_label, struct defined_label (*definedLabel)[], int definedLabel_count) {
    for (int i = 0; i < definedLabel_count; i++) {
        if (find_label->name_len == (*definedLabel)[i].name_len &&
            strncmp(find_label->name, (*definedLabel)[i].name, (*definedLabel)[i].name_len) == 0) {
            return i;
        }
    }
    return -1;
}
