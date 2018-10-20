# im-manager
Cross platform input source manager for Windows/macOS/Linux(ibus/fcitx/XIM)

# Current Progress
* [x] macOS
* [x] ibus (Linux)
* [x] fcitx(Linux)
* [ ] Windows

# Bindings
* [x] [Java](https://github.com/rieonke/im-manager/tree/master/binding/im-manager-jvm)
* [ ] Python
* [ ] NodeJS

# Usages
## Building from source
```bash
git clone https://github.com/rieonke/im-manager.git
cd im-manager
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && make test
```

## Core api example

```c
#include <im_manager.h> //include the im_manager header
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

  setlocale(LC_ALL,"en_US.utf8");

  imm_input_source_list *isl = imm_get_input_source_list(); //get all the available input methods

  for (int i = 0; i < isl->count; ++i) { //iterate all the input methods
    imm_input_source p = isl->iss[i];
    print_is(p);
  }

  imm_free_input_source_list(isl);  // don't forget to release the memory


  imm_input_source *is = imm_get_current_input_source();  // get current active input method
  
  print_is(*is);
  imm_free_input_source(is); // don't forget to release the memory

  if (!imm_select(TARGET_IS_ID)) {
    return -1;
  }

  imm_input_source *by_name = imm_get_input_source_by_id(TARGET_IS_ID); // get the specified input method by given id
  print_is(*by_name);
  imm_free_input_source(by_name); // don't forget to release the memory

  return 0;
}

```

