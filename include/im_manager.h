#ifndef IM_MANAGER_IM_MANAGER_H
#define IM_MANAGER_IM_MANAGER_H

#define imm_bool_false 0
#define imm_bool_true 1

typedef int imm_bool;

/**
 * simple input source struct
 */
struct imm_input_source {
    char *name;
    char *id;
    char *icon;
    imm_bool selectable;
};

typedef struct imm_input_source imm_input_source;

/**
 * input source as list
 */
struct imm_input_source_list {
    imm_input_source *iss;
    unsigned int count;
};

typedef struct imm_input_source_list imm_input_source_list;


/**
 * get all of the input sources as list.
 * @return all input sources
 */
imm_input_source_list *imm_get_input_source_list();

/**
 * select the specified input source by given id.
 * @param id input source identifier
 * @return select success status
 */
imm_bool imm_select(const char *id);

/**
 * get current selected input source of system.
 */
imm_input_source *imm_get_current_input_source();

/**
 * get the input source by given id
 * @param id the input source id
 * @return input source
 */
imm_input_source *imm_get_input_source_by_id(const char *id);

/**
 * free the imm_input_source_list
 * @param isl pointer
 */
void imm_free_input_source_list(imm_input_source_list *isl);

/**
 * free the imm_input_source
 * @param is pointer
 */
void imm_free_input_source(imm_input_source *is);

#endif //IM_MANAGER_IM_MANAGER_H
