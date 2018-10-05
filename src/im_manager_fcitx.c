#include <stdio.h>
#include <stdlib.h>
#include <fcitx-gclient/fcitxinputmethod.h>
#include <fcitx-utils/utils.h>
#include <im_manager.h>
#include <glib-2.0/glib.h>
#include "common.h"

#define fcitx_ctx_t FcitxInputMethod
#define g_err_t GError
#define g_ptr_arr_t GPtrArray
#define fcitx_im_item_t FcitxIMItem

fcitx_ctx_t *_imm_get_fcitx_ctx() {
  g_err_t *err = NULL;
  fcitx_ctx_t *im;

  im = fcitx_input_method_new(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
      fcitx_utils_get_display_number(), NULL, &err);
  if (im == NULL) {
    g_error_free(err);
    return NULL;
  }
  return im;
}

void __imm_g_ptr_arr_size_cb(gpointer data, unsigned long *size) {

  if (data) {
    *size = *size + 1;
  }
}

unsigned long _imm_g_ptr_arr_size(g_ptr_arr_t *arr) {

  unsigned long size = 0;

  g_ptr_array_foreach(arr, (GFunc) __imm_g_ptr_arr_size_cb, &size);

  return size;
}

void _imm_extract_input_source_cb(fcitx_im_item_t *item, imm_input_source_list *isl) {
  imm_input_source *iss = isl->iss + isl->count;
  _imm_input_source_construct(iss, item->name, item->unique_name, "none", item->enable);
  isl->count++;

}

gboolean __imm_find_target_is(gpointer a, gpointer b) {
  fcitx_im_item_t *im = a;
  const char *target = b;

  if (strcmp(im->unique_name,target) == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * get all of the input sources as list.
 * @return all input sources
 */
imm_input_source_list *imm_get_input_source_list() {

  fcitx_ctx_t *im = _imm_get_fcitx_ctx();
  if (im == NULL) {
    return NULL;
  }

  g_ptr_arr_t *im_list;
  imm_input_source_list *isl;

  isl = (imm_input_source_list *) malloc(sizeof(imm_input_source_list));
  memset(isl, 0, sizeof(imm_input_source_list));

  im_list = fcitx_input_method_get_imlist(im);
  unsigned long size = _imm_g_ptr_arr_size(im_list);
  if (size == 0) {
    isl->count = 0;
  } else {
    isl->iss = malloc(size * sizeof(imm_input_source));
    memset(isl->iss, 0, size * sizeof(imm_input_source));
    g_ptr_array_foreach(im_list, (GFunc) _imm_extract_input_source_cb, isl);
  }

  g_object_unref(im);

  return isl;
}


/**
 * select the specified input source by given id.
 * @param id input source identifier
 * @return select success status
 */
imm_bool imm_select(const char *id) {

  fcitx_ctx_t *im = NULL;
  g_ptr_arr_t *im_list = NULL;
  int target_index = -1;
  fcitx_im_item_t *target = NULL;

  im = _imm_get_fcitx_ctx();
  if (!im) {
    return imm_bool_false;
  }

  im_list = fcitx_input_method_get_imlist(im);

  //find the target input method
  g_ptr_array_find_with_equal_func(im_list,id,(GCompareFunc)__imm_find_target_is,(unsigned int*)&target_index);

  if (target_index == -1) {
    g_object_unref(im);
    return imm_bool_false;
  }

  target = g_ptr_array_index(im_list,target_index);

  if (target != NULL) {
    fcitx_im_item_t * pre = NULL;
    pre = g_ptr_array_index(im_list,0);

    //if target is the first, skip. else move is to the top
    if (strcmp(pre->unique_name,target->unique_name) != 0) {
      fcitx_im_item_t *s = malloc(sizeof(fcitx_im_item_t));

      memmove(s,pre, sizeof(fcitx_im_item_t));
      memmove(pre,target, sizeof(fcitx_im_item_t));
      memmove(target,s, sizeof(fcitx_im_item_t));

      free(s);

      fcitx_input_method_set_imlist(im, im_list);
    }

  }

  g_object_unref(im);
  return imm_bool_true;
}

/**
 * get current selected input source of system.
 */
imm_input_source *imm_get_current_input_source() {

  fcitx_ctx_t *im = NULL;
  g_ptr_arr_t *im_list = NULL;
  fcitx_im_item_t *target = NULL;
  imm_input_source *iss = NULL;

  im = _imm_get_fcitx_ctx();
  if (!im) {
    return NULL;
  }

  im_list = fcitx_input_method_get_imlist(im);
  target = g_ptr_array_index(im_list,0);
  if (!target) {
    g_object_unref(im);
    return NULL;
  }

  iss = malloc(sizeof(imm_input_source));
  _imm_input_source_construct(iss,target->name,target->unique_name,"none",target->enable);

  g_object_unref(im);
  return iss;

}

/**
 * get the input source by given id
 * @param id the input source id
 * @return input source
 */
imm_input_source *imm_get_input_source_by_id(const char *id) {

  fcitx_ctx_t *im = NULL;
  g_ptr_arr_t *im_list = NULL;
  fcitx_im_item_t *target = NULL;
  imm_input_source *iss = NULL;
  int index = -1;

  im = _imm_get_fcitx_ctx();
  if (!im) {
    return NULL;
  }

  im_list = fcitx_input_method_get_imlist(im);
  g_ptr_array_find_with_equal_func(im_list,id,(GEqualFunc)__imm_find_target_is,(unsigned int *)&index);

  if (index == -1) {
    g_object_unref(im);
    return NULL;
  }

  target = g_ptr_array_index(im_list,index);
  if (!target) {
    g_object_unref(im);
    return NULL;
  }

  iss = malloc(sizeof(imm_input_source));
  _imm_input_source_construct(iss,target->name,target->unique_name,"none",target->enable);

  g_object_unref(im);
  return iss;

}
