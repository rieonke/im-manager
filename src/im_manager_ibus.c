#include <im_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <ibus.h>
#include "common.h"

IBusBus *_imm_init_ibus();

/**
 * get all the input sources.
 * @return input source list
 */
imm_input_source_list *imm_get_input_source_list() {

  IBusBus *bus = _imm_init_ibus();

  if (bus == NULL) {
    return NULL;
  }

  GList *engines = NULL;
  IBUS_TYPE_ENGINE_DESC;

  engines = ibus_bus_list_engines(bus);

  imm_input_source *iss = NULL;
  imm_input_source_list *isl = NULL;

  iss = (imm_input_source *) malloc(sizeof(imm_input_source));
  isl = (imm_input_source_list *) malloc(sizeof(imm_input_source_list));

  unsigned int count = 0;
  for (; engines; engines = g_list_next(engines)) {
    if (count != 0) {
      iss = (imm_input_source *) realloc(iss, (count + 1) * sizeof(imm_input_source));
    }

    IBusEngineDesc *engine_desc = IBUS_ENGINE_DESC(engines->data);
    const char *id = ibus_engine_desc_get_name(engine_desc);
    const char *name = ibus_engine_desc_get_longname(engine_desc);
    const char *icon = ibus_engine_desc_get_icon(engine_desc);

    imm_input_source *p = iss + count;

    _imm_input_source_construct(p, name, id, icon, imm_bool_true); //todo selectable

    count++;

    g_object_unref(engine_desc);
  }

  isl->count = count;
  isl->iss = iss;

  g_list_foreach(engines, (GFunc) g_object_unref, NULL);
  g_list_free(engines);

  g_object_unref(bus);

  return isl;

}


imm_input_source *imm_get_current_input_source() {

  IBusBus *bus = _imm_init_ibus();

  if (bus == NULL) {
    return NULL;
  }

  GVariant *result = NULL, *obj = NULL;
  IBusEngineDesc *desc = NULL;
  imm_input_source *is = NULL;

  result = ibus_bus_get_ibus_property(bus, "GlobalEngine");
  if (result == NULL)
    goto clean_up;

  obj = g_variant_get_variant(result);
  if (obj == NULL)
    goto clean_up;

  desc = IBUS_ENGINE_DESC(ibus_serializable_deserialize(obj));
  if (desc == NULL)
    goto clean_up;

  const char *name = ibus_engine_desc_get_longname(desc);
  const char *id = ibus_engine_desc_get_name(desc);
  const char *icon = ibus_engine_desc_get_icon(desc);

  is = malloc(sizeof(imm_input_source));

  _imm_input_source_construct(is, name, id, icon, imm_bool_true);

clean_up:
  {
    if (obj != NULL)
      g_variant_unref(obj);
    if (result != NULL)
      g_variant_unref(result);
    if (desc != NULL)
      g_object_unref(desc);
    if (bus != NULL)
      g_object_unref(bus);
  };

  return is;
}

imm_bool imm_select(const char *id) {

  IBusBus *bus = _imm_init_ibus();
  if (bus == NULL) {
    return imm_bool_false;
  }

  imm_input_source *is = imm_get_input_source_by_id(id);
  if (is == NULL) {
    g_object_unref(bus);
    return imm_bool_false;
  }

  imm_free_input_source(is);

  imm_bool i = ibus_bus_set_global_engine(bus, id);

  g_object_unref(bus);
  if (!i) {
    return imm_bool_false;
  }

  return imm_bool_true;

}


imm_input_source *imm_get_input_source_by_id(const char *id) {

  return __default_imm_get_input_source_by_id(id);
}


IBusBus *_imm_init_ibus() {

  IBusBus *bus = NULL;

  ibus_init();
  bus = ibus_bus_new();

  if (!ibus_bus_is_connected(bus)) {

    g_object_unref(bus);
    return NULL;
  }

  return bus;
}

