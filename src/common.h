#ifndef IM_MANAGER_COMMON_H
#define IM_MANAGER_COMMON_H

#include <im_manager.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * helper: construct imm_input_source fileds
 * @param is
 * @param name
 * @param id
 * @param icon
 */
void _imm_input_source_construct(imm_input_source *is,
                                 const char *name,
                                 const char *id,
                                 const char *icon,
                                 imm_bool selectable) {

    size_t sz_id = (strlen(id) + 1) * sizeof(char);
    size_t sz_name = (strlen(name) + 1) * sizeof(char);
    size_t sz_icon = (strlen(icon) + 1) * sizeof(char);

    is->id = malloc(sz_id);
    is->name = malloc(sz_name);
    is->icon = malloc(sz_icon);

    is->selectable = selectable;

    memset(is->id, 0, sz_id);
    memset(is->name, 0, sz_name);
    memset(is->icon, 0, sz_icon);

    strcpy(is->name, name);
    strcpy(is->id, id);
    strcpy(is->icon, icon);
}

/**
 * helper: free imm_input_source
 * @param is
 */
void imm_free_input_source(imm_input_source *is) {

    free(is->name);
    free(is->id);
    free(is->icon);

    free(is);
}

/**
 * helper: free the input_source_list
 * @param isl input_source_list to free
 */
void imm_free_input_source_list(imm_input_source_list *isl) {
    imm_free_input_source(isl->iss);
    free(isl);
}

/**
 * helper: default imm_get_input_source_by_id impl
 * @param id
 * @return
 */
imm_input_source *__default_imm_get_input_source_by_id(const char *id) {

    imm_input_source_list *isl = imm_get_input_source_list();
    if (isl == NULL) {
        return NULL;
    }

    imm_input_source *selected = NULL;

    for (int i = 0; i < isl->count; ++i) {
        imm_input_source *p = (isl->iss + i);
        if (strcmp(p->id, id) == 0) {
            selected = malloc(sizeof(imm_input_source));
            memset(selected, 0, sizeof(imm_input_source));
            _imm_input_source_construct(selected,p->name,p->id,p->icon,p->selectable);
            break;
        }
    }

    imm_free_input_source_list(isl);

    return selected;

}

#endif //IM_MANAGER_COMMON_H
