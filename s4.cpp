#include <fstream>
#include <iostream>

#include "backends/bmv2/psa_switch/options.h"
#include "ir/ir.h"
#include "frontends/common/options.h"
#include "frontends/common/parseInput.h"
#include "frontends/common/applyOptionsPragmas.h"
#include "frontends/p4/frontend.h"
#include "backends/bmv2/psa_switch/midend.h"
#include "lib/error.h"
#include "lib/gc.h"
#include "lib/nullstream.h"
/* #include "wasmc/wasmc.h" */

#include "options.h"
#include "p4_backend/backend.h"
#include "wasm.h"


int p4Compile(BMV2::BMV2Options &options) {
    auto hook = options.getDebugHook();


    /*
     * Parser
     */
    auto program = P4::parseP4File(options);
    if (program == nullptr || ::errorCount() > 0) {
        std::cerr << "Can't parse P4 file " << options.file << std::endl;
        return 1;
    }


    /*
     * FrontEnd
     */
    P4::P4COptionPragmaParser options_pragma_parser;
    program->apply(P4::ApplyOptionsPragmas(options_pragma_parser));

    P4::FrontEnd frontend;
    frontend.addDebugHook(hook);
    program = frontend.run(options, program);
    if (::errorCount() > 0) {
        return 1;
    }


    /*
     * MidEnd
     */
    BMV2::PsaSwitchMidEnd midend(options);
    midend.addDebugHook(hook);
    auto toplevel = midend.process(program);
    if (::errorCount() > 0) {
        return 1;
    }


    /*
     * BackEnd
     */
    /* S4::Backend backend(&options, &midend.refMap, &midend.typeMap, &midend.enumMap); */
    S4::Backend backend(options, &midend.refMap, &midend.typeMap, &midend.enumMap);
    backend.process(toplevel);

    return 0;
}


int main(int argc, char *const argv[]) {
    setup_gc_logging();

    /*
     * S4 Option
     */
    S4::S4Context s4Context;
    auto& s4Options = s4Context.options();
    s4Options.langVersion = CompilerOptions::FrontendVersion::P4_16;
    s4Options.compilerVersion = "0.0.1";
    if (s4Options.process(argc, argv) != nullptr) {
        s4Options.setInputFile();
    }

    if (s4Options.wasmPaths.size() > 0) {
        for (auto wasmPath : s4Options.wasmPaths) {
            S4::wasmCompile(wasmPath);
        }
    }

    /*
     * PSA Option
     */
    AutoCompileContext autoPsaSwitchContext(new BMV2::PsaSwitchContext);
    auto& psaOptions = BMV2::PsaSwitchContext::get().options();
    psaOptions = dynamic_cast<BMV2::PsaSwitchOptions&>(s4Options);

    if (::errorCount() > 0) {
        return 1;
    }

    
    int res = p4Compile(psaOptions);
    if (res > 0) {
        return res;
    }


    // link...

    return 0;
}
