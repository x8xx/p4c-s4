#include "ir/ir.h"
#include "native_object.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

namespace S4 {
class Backend {
    public:
        native_object *process(const IR::ToplevelBlock *toplevel);
};
}
