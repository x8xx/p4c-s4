#ifndef BACKENDS_S4_SWITCH_OPTIONS_H_
#define BACKENDS_S4_SWITCH_OPTIONS_H_


#include "backends/bmv2/psa_switch/options.h"
/* #include "frontends/common/options.h" */

namespace S4 {

/* class S4Options : public CompilerOptions { */
class S4Options : public BMV2::PsaSwitchOptions {
 public:
    cstring wasm = nullptr;
    S4Options() {
      registerOption("-w", "webassembly",
              [this](const char* arg) { this->wasm = arg; return true; },
              "Input WebAssemlby(.wasm)");
     }
};

using S4Context = P4CContextWithOptions<S4Options>;

};


#endif
