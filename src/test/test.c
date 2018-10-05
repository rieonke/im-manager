#include <im_manager.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#ifdef ENABLE_IBUS
#define TARGET_IS_ID "xkb:us::eng"
#elif ENABLE_FCITX
#define TARGET_IS_ID "pinyin"
#elif ENABLE_OSX
#define TARGET_IS_ID "com.apple.keylayout.ABC"
#endif

void print_is(imm_input_source p) {
  assert(p.name);
  assert(p.id);
  assert(p.icon);

  assert(strcmp(p.name, "") != 0);
  assert(strcmp(p.id, "") != 0);
  assert(strcmp(p.icon, "") != 0);

  if (p.selectable)
    printf("name: %s\nid: %s\nicon: %s\n\n", p.name, p.id, p.icon);
}

int main() {


  //    for (int j = 0; j < 10; ++j) {

  imm_input_source_list *isl = imm_get_input_source_list();

  for (int i = 0; i < isl->count; ++i) {
    imm_input_source p = isl->iss[i];
    print_is(p);
  }

  imm_free_input_source_list(isl);
  //    }


  //    for (int k = 0; k < 10; ++k) {
  imm_input_source *is = imm_get_current_input_source();
  print_is(*is);
  imm_free_input_source(is);
  //    }

  if (!imm_select(TARGET_IS_ID)) {
    return -1;
  }

  imm_input_source *by_name = imm_get_input_source_by_id(TARGET_IS_ID);
  print_is(*by_name);
  imm_free_input_source(by_name);

  return 0;
}
