#include <node_api.h>
#include <im_manager.h>
#include <string.h>
#include <stdio.h>

#define CHECK_STATUS \
  if (status != napi_ok) { \
    napi_throw_error(env, NULL, "N-API call failed"); \
    return NULL; \
  }

void* __construct_js_im(imm_input_source *is, napi_value *obj, napi_env env) {

  napi_status status;

  napi_value im_name;
  napi_value im_id;
  napi_value im_icon;
  napi_value im_selectable;

  status = napi_create_string_utf8(env, is->name, NAPI_AUTO_LENGTH, &im_name);
  CHECK_STATUS;

  status = napi_create_string_utf8(env, is->id, NAPI_AUTO_LENGTH, &im_id);
  CHECK_STATUS;

  status = napi_create_string_utf8(env, is->icon, NAPI_AUTO_LENGTH, &im_icon);
  CHECK_STATUS;

  status = napi_get_boolean(env, is->selectable, &im_selectable);
  CHECK_STATUS;

  status = napi_create_object(env, obj);
  CHECK_STATUS;

  status = napi_set_named_property(env, *obj, "name", im_name);
  CHECK_STATUS;

  status = napi_set_named_property(env, *obj, "id", im_id);
  CHECK_STATUS;

  status = napi_set_named_property(env, *obj, "icon", im_icon);
  CHECK_STATUS;

  status = napi_set_named_property(env, *obj, "selectable", im_selectable);
  CHECK_STATUS;

  return NULL; // return null for check node_api status
}

napi_value Current(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1; 
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  CHECK_STATUS;

  napi_value cb = args[0];

  imm_input_source *is = imm_get_current_input_source();


  napi_value argv;
  if (is != NULL) {
    __construct_js_im(is,&argv,env);
    imm_free_input_source(is);
  }else {
    status = napi_get_undefined(env,&argv);
    CHECK_STATUS;
  }

  napi_value global;
  status = napi_get_global(env, &global);
  CHECK_STATUS;


  napi_value result;
  status = napi_call_function(env, global, cb, 1, &argv, &result);
  CHECK_STATUS;

  return NULL;
}

napi_value Select(napi_env env, napi_callback_info info) {

  napi_status status;

  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  CHECK_STATUS;

  napi_value global;
  status = napi_get_global(env, &global);
  CHECK_STATUS;

  napi_value id = args[0];
  napi_value cb = args[1];

  if (id == NULL || cb == NULL) {
    napi_throw_error(env, NULL, "select: at least two parameter required!"); 
    return NULL;
  }

  size_t len;
  status = napi_get_value_string_utf8(env, id, NULL, 0, &len);
  CHECK_STATUS;

  imm_bool sr = imm_bool_false;

  if (len >= 0) {
    char dest_id[len + 1];
    memset(dest_id,0,(len + 1) * sizeof(char));
    napi_get_value_string_utf8(env, id, dest_id, len + 1, &len);

    sr = imm_select(dest_id);
  }

  napi_value n_sr;
  status = napi_get_boolean(env,sr,&n_sr);
  CHECK_STATUS;

  napi_value result;
  status = napi_call_function(env, global, cb, 1, &n_sr, &result);
  CHECK_STATUS;

  return NULL;
}

napi_value Get(napi_env env, napi_callback_info info) {

  napi_status status;

  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  CHECK_STATUS;

  napi_value id = args[0];
  napi_value cb = args[1];
  napi_value obj;
  imm_input_source * is;

  if (id == NULL || cb == NULL) {
    napi_throw_error(env, NULL, "get: at least two parameter required!"); 
    return NULL;
  }

  size_t len;
  status = napi_get_value_string_utf8(env, id, NULL, 0, &len);
  CHECK_STATUS;

  imm_bool found = imm_bool_false;

  if (len > 0) {
    char dest_id[len + 1];
    memset(dest_id,0,(len + 1) * sizeof(char));
    napi_get_value_string_utf8(env, id, dest_id, len + 1, &len);

    is = imm_get_input_source_by_id(dest_id);
    if (is != NULL) {
      __construct_js_im(is, &obj, env);
      found = imm_bool_true;
    }

  }

  CHECK_STATUS;
  napi_value global;
  status = napi_get_global(env, &global);
  CHECK_STATUS;

  if (!found) {
    napi_get_undefined(env,&obj);
  }

  napi_value result;
  status = napi_call_function(env, global, cb, 1, &obj, &result);
  CHECK_STATUS;

  if (is != NULL) {
    imm_free_input_source(is);
  }

  return NULL;
}

napi_value List(napi_env env, napi_callback_info info) {

  napi_status status;

  size_t argc = 1;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  CHECK_STATUS;

  napi_value cb = args[0];
  imm_input_source_list *isl;
  imm_bool success = imm_bool_false;

  napi_value argv;

  isl = imm_get_input_source_list();

  if (isl != NULL) {
    status = napi_create_array_with_length(env, isl->count, &argv);
    CHECK_STATUS;

    for (int i = 0; i < isl->count; i++) {

      imm_input_source *is = isl->iss + i;
      if (is != NULL) {
        napi_value obj;
        __construct_js_im(is,&obj,env);

        status = napi_set_element(env, argv, i, obj);
        CHECK_STATUS;
      }
    }

    imm_free_input_source_list(isl);

  } else {
    status = napi_create_array_with_length(env, 0, &argv);
    CHECK_STATUS;
  }

  napi_value global;
  status = napi_get_global(env, &global);
  CHECK_STATUS;

  napi_value result;
  status = napi_call_function(env, global, cb, 1, &argv, &result);
  CHECK_STATUS;

  return NULL;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;

  napi_value fn_list;
  status = napi_create_function(env, NULL, 0, List, NULL, &fn_list);
  CHECK_STATUS;

  napi_value fn_current;
  status = napi_create_function(env, NULL, 0, Current, NULL, &fn_current);
  CHECK_STATUS;

  napi_value fn_select;
  status = napi_create_function(env, NULL, 0, Select, NULL, &fn_select);
  CHECK_STATUS;

  napi_value fn_get;
  status = napi_create_function(env, NULL, 0, Get, NULL, &fn_get);
  CHECK_STATUS;

  status = napi_set_named_property(env, exports, "current", fn_current);
  CHECK_STATUS;

  status = napi_set_named_property(env, exports, "select", fn_select);
  CHECK_STATUS;

  status = napi_set_named_property(env, exports, "list", fn_list);
  CHECK_STATUS;

  status = napi_set_named_property(env, exports, "get", fn_get);
  CHECK_STATUS;

  return exports;

}

NAPI_MODULE(ImManager, Init)
