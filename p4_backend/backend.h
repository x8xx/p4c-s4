#include "frontends/p4/typeMap.h"
#include "ir/ir.h"
#include "../native_object.h"
#include "backends/bmv2/psa_switch/options.h"
/* #include "backends/bmv2/psa_switch/psaSwitch.h" */
#include "backends/bmv2/psa_switch/psaProgramStructure.h"
#include "backends/bmv2/common/programStructure.h"
#include "frontends/p4/evaluator/evaluator.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

namespace S4 {
class Backend {
    private:
        BMV2::BMV2Options options;
        P4::ReferenceMap *refMap;
        P4::TypeMap *typeMap;
        P4::ConvertEnums::EnumMapping *enumMap;

    public:
        Backend(BMV2::BMV2Options &options);
        Backend(BMV2::BMV2Options &options, P4::ReferenceMap *refMap, P4::TypeMap *typeMap, P4::ConvertEnums::EnumMapping *enumMap);
        native_object *process(const IR::ToplevelBlock *toplevel);
};
}
