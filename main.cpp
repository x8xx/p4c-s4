#include <fstream>
#include <iostream>

#include "options.h"
#include "ir/ir.h"
#include "frontends/common/options.h"
#include "frontends/common/parseInput.h"
#include "frontends/p4/frontend.h"
#include "backends/bmv2/psa_switch/midend.h"
#include "lib/error.h"
#include "lib/gc.h"
#include "lib/nullstream.h"




int compile(CompilerOptions& options) {
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


    /*
     * BackEnd
     */

    return 0;
}


int main(int argc, char *const argv[]) {
    setup_gc_logging();

    /*
     * Option
     */
    AutoCompileContext autoS4Context(new S4::S4Context);
    auto& options = S4::S4Context::get().options();
    options.langVersion = CompilerOptions::FrontendVersion::P4_16;
    options.compilerVersion = "0.0.1";

    if (options.process(argc, argv) != nullptr) {
        options.setInputFile();
    }

    if (::errorCount() > 0) {
        return 1;
    }


    return compile(options);
}
