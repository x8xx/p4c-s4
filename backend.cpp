#include "backend.h"

namespace S4 {
native_object *Backend::process(const IR::ToplevelBlock *toplevel) {
    llvm::LLVMContext context;
    auto module = new llvm::Module("p4", context);

    std::cout << "name: " <<  toplevel->getName() << std::endl;
    return nullptr;
}
}
