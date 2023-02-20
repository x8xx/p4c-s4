#ifndef BACKENDS_S4_SWITCH_OPTIONS_H_
#define BACKENDS_S4_SWITCH_OPTIONS_H_


#include "backends/bmv2/psa_switch/options.h"

namespace S4 {

class S4Options : public BMV2::PsaSwitchOptions {
 public:
    cstring outputFile = nullptr;
    S4Options() {
      /* registerOption("-o", "outfile", */
      /*         [this](const char* arg) { this->outputFile = arg; return true; }, */
      /*         "Write output to outfile"); */
     }
};

using S4Context = P4CContextWithOptions<S4Options>;

};


#endif
