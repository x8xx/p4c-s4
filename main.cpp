#include <fstream>
#include <iostream>

#include "options.h"
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
#include "wasmc/wasmc.h"


int wasmCompile() {
    return 0;
}


int p4Compile(CompilerOptions& options) {
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
    P4::P4COptionPragmaParser optionsPragmaParser;
    program->apply(P4::ApplyOptionsPragmas(optionsPragmaParser));

    P4::FrontEnd frontend;
    frontend.addDebugHook(hook);
    program = frontend.run(options, program);
    if (::errorCount() > 0) {
        return 1;
    }


    /*
     * MidEnd
     */
    BMV2::PsaSwitchMidEnd midEnd(options);
    midEnd.addDebugHook(hook);
    auto toplevel = midEnd.process(program);
    if (::errorCount() > 0) {
        return 1;
    }

    std::cout << "name: " <<  toplevel->getName() << std::endl;

    /*
     * BackEnd
     */

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

    if (s4Options.wasm != nullptr) {
        std::cout << s4Options.wasm << std::endl;
    } else {
        std::cout << "null desu..." << std::endl;
    }

    wasmCompile();



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
