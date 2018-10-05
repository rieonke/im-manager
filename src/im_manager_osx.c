#include <im_manager.h>
#include <Carbon/Carbon.h>
#include "common.h"

#define osx_enc_utf8 kCFStringEncodingUTF8
#define osx_cmp_insensitive kCFCompareCaseInsensitive
#define osx_tis_cate_keyboard kTISCategoryKeyboardInputSource
#define osx_tis_prop_is_selectable kTISPropertyInputSourceIsSelectCapable
#define osx_tis_prop_is_id kTISPropertyInputSourceID
#define osx_tis_prop_name kTISPropertyLocalizedName
#define osx_tis_prop_cate kTISPropertyInputSourceCategory
#define osx_tis_prop_enabled kTISPropertyInputSourceIsEnabled

#define osx_true kCFBooleanTrue
#define osx_false kCFBooleanFalse

#define osx_bool_get_value CFBooleanGetValue
#define osx_str_cmp CFStringCompare
#define osx_str_get_cstr CFStringGetCString
#define osx_str_len CFStringGetLength
#define osx_str_of_cstr CFStringCreateWithCString
#define osx_dict_mut_new CFDictionaryCreateMutable
#define osx_dict_add_value CFDictionaryAddValue
#define osx_array_count CFArrayGetCount
#define osx_array_get_value_at CFArrayGetValueAtIndex
#define osx_tis_new_is_list TISCreateInputSourceList
#define osx_tis_get_is_prop TISGetInputSourceProperty
#define osx_tis_enable_is TISEnableInputSource
#define osx_tis_select_is TISSelectInputSource
#define osx_tis_copy_current_kbis TISCopyCurrentKeyboardInputSource

#define osx_cf_release CFRelease
#define osx_cf_retain CFRetain

typedef CFStringRef osx_string;
typedef CFBooleanRef osx_bool;
typedef TISInputSourceRef osx_input_source;
typedef CFMutableDictionaryRef osx_mut_dict;
typedef CFArrayRef osx_array;
typedef CFTypeRef osx_ctype;

osx_ctype _imm_osx_retain(osx_ctype ptr);

void _imm_osx_free(osx_ctype ptr);

char *_imm_osx_str_get_cstr(osx_string str);

osx_input_source _imm_get_osx_input_source(const char *id);

/**
 * get all of the input sources as list.
 * @return all input sources
 */
imm_input_source_list *imm_get_input_source_list() {

  osx_mut_dict o_properties = NULL;
  osx_array o_all_ime = NULL;
  signed long count_all_ime = 0;

  o_properties = osx_dict_mut_new(nil, 1, nil, nil);
  osx_dict_add_value(o_properties, osx_tis_prop_is_selectable, osx_true);

  o_all_ime = osx_tis_new_is_list(NULL, false);
  count_all_ime = osx_array_count(o_all_ime);

  imm_input_source_list *isl = NULL;
  imm_input_source *iss = NULL;

  if (count_all_ime >= 1) {

    isl = malloc(sizeof(imm_input_source_list));
    memset(isl, 0, sizeof(imm_input_source_list));

    unsigned int count = 0;
    for (unsigned int i = 0; i < count_all_ime; i++) {

      osx_input_source o_tis = (osx_input_source) osx_array_get_value_at(o_all_ime, i);

      osx_string o_id = NULL;
      osx_string o_name = NULL;
      osx_string o_cate = NULL;
      osx_bool o_slct = NULL;

      char *str_id = NULL;
      char *str_name = NULL;

      o_id = osx_tis_get_is_prop(o_tis, osx_tis_prop_is_id);
      o_name = osx_tis_get_is_prop(o_tis, osx_tis_prop_name);
      o_cate = osx_tis_get_is_prop(o_tis, osx_tis_prop_cate);
      o_slct = osx_tis_get_is_prop(o_tis, osx_tis_prop_is_selectable);

      o_id = _imm_osx_retain(o_id);
      o_name = _imm_osx_retain(o_name);
      o_cate = _imm_osx_retain(o_cate);
      o_slct = _imm_osx_retain(o_slct);

      if (osx_bool_get_value(o_slct) && !osx_str_cmp(o_cate, osx_tis_cate_keyboard, osx_cmp_insensitive)) {

        str_id = _imm_osx_str_get_cstr(o_id);
        str_name = _imm_osx_str_get_cstr(o_name);

        if (str_id == NULL || str_name == NULL) {
          printf("ID NULL");
        }

        iss = (imm_input_source *) realloc(iss, (count + 1) * sizeof(imm_input_source));

        imm_input_source *p = iss + count;
        _imm_input_source_construct(p, str_name, str_id, "none", imm_bool_true); //todo selectable

        free(str_id);
        free(str_name);

        count++;
      }

      _imm_osx_free(o_id);
      _imm_osx_free(o_name);
      _imm_osx_free(o_cate);
      _imm_osx_free(o_slct);
      _imm_osx_free(o_tis);
    }
    isl->iss = iss;
    isl->count = count;
  }

  return isl;
}

/**
 * select the specified input source by given id.
 * @param id input source identifier
 * @return select success status
 */
imm_bool imm_select(const char *id) {

  osx_input_source o_tis = _imm_get_osx_input_source(id);

  if (o_tis) {
    osx_bool enabled = osx_tis_get_is_prop(o_tis, osx_tis_prop_enabled);
    _imm_osx_retain(enabled);
    if (enabled == osx_false) {
      osx_tis_enable_is(o_tis);
    }
    _imm_osx_free(enabled);
    osx_tis_select_is(o_tis);
    _imm_osx_free(o_tis);
    return imm_bool_true;
  }
  return imm_bool_false;
}

/**
 * get current selected input source of system.
 */
imm_input_source *imm_get_current_input_source() {

  osx_input_source o_tis = NULL;
  osx_string o_id = NULL;
  osx_string o_name = NULL;

  char *str_id = NULL;
  char *str_name = NULL;

  imm_input_source *iss = NULL;

  o_tis = osx_tis_copy_current_kbis();
  o_id = osx_tis_get_is_prop(o_tis, osx_tis_prop_is_id);
  o_name = osx_tis_get_is_prop(o_tis, osx_tis_prop_name);

  o_id = _imm_osx_retain(o_id);
  o_name = _imm_osx_retain(o_name);

  str_id = _imm_osx_str_get_cstr(o_id);
  str_name = _imm_osx_str_get_cstr(o_name);

  iss = (imm_input_source *) malloc(sizeof(imm_input_source));
  _imm_input_source_construct(iss, str_name, str_id, "none", imm_bool_true); //todo icon&selectable

  free(str_id);
  free(str_name);


  _imm_osx_free(o_tis);
  _imm_osx_free(o_id);
  _imm_osx_free(o_name);

  return iss;
}

/**
 * get the input source by given id
 * @param id the input source id
 * @return input source
 */
imm_input_source *imm_get_input_source_by_id(const char *id) {
  return __default_imm_get_input_source_by_id(id);
}

/**
 * helper: convert osx_string to c string
 * @param str
 * @return
 */
char *_imm_osx_str_get_cstr(osx_string str) {
  int len = (int) osx_str_len(str) * 4 + 1;
  char *out = (char *) (malloc(len * sizeof(char)));
  osx_str_get_cstr(str, out, len, osx_enc_utf8);
  return out;
}

/**
 * helper: cf_retain osx obj
 * @param ptr
 * @return
 */
osx_ctype _imm_osx_retain(osx_ctype ptr) {
  return osx_cf_retain(ptr);
}

/**
 * helper: free osx obj
 * @param ptr
 */
void _imm_osx_free(osx_ctype ptr) {

  if (ptr != NULL) {
    osx_cf_release(ptr);
    ptr = NULL;
  }
}

/**
 * helper: get osx_input_source by id
 * @param id
 * @return
 */
osx_input_source _imm_get_osx_input_source(const char *id) {
  osx_string o_id = NULL;
  osx_mut_dict o_prop = NULL;
  osx_array o_is_arr = NULL;

  o_id = osx_str_of_cstr(nil, id, osx_enc_utf8);
  o_prop = osx_dict_mut_new(nil, 1, nil, nil);
  osx_dict_add_value(o_prop, osx_tis_prop_is_id, o_id);
  o_is_arr = osx_tis_new_is_list(o_prop, true);

  _imm_osx_free(o_id);
  _imm_osx_free(o_prop);

  if (!o_is_arr) {
    return NULL;
  }

  osx_input_source tis = (osx_input_source) osx_array_get_value_at(o_is_arr, 0);
  _imm_osx_retain(tis);
  _imm_osx_free(o_is_arr);
  return tis;
}

