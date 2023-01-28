#include <fstream>
#include <iostream>
#include "ir/ir.h"

#include "frontends/common/options.h"
#include "frontends/common/parseInput.h"
#include "frontends/p4/frontend.h"

#include "lib/nullstream.h"


class S4Options : public CompilerOptions {
 public:
    cstring outputFile = nullptr;
    S4Options() {
      registerOption("-o", "outfile",
              [this](const char* arg) { this->outputFile = arg; return true; },
              "Write output to outfile");
     }
};

using S4Context = P4CContextWithOptions<S4Options>;


int main(int argc, char *const argv[]) {
    std::cout << "heloworld" << std::endl;

    AutoCompileContext autoS4Context(new S4Context);
    auto& options = S4Context::get().options();
    options.langVersion = CompilerOptions::FrontendVersion::P4_16;
    options.compilerVersion = "0.0.1";

    if (options.process(argc, argv) != nullptr)
        options.setInputFile();
    if (::errorCount() > 0)
        return 1;

    auto program = P4::parseP4File(options);

    if (program == nullptr || ::errorCount() > 0) {
        std::cerr << "Can't parse P4 file " << options.file << std::endl;
        return 1;
    }

    return 0;
}
