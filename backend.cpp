#include "backend.h"

namespace S4 {

Backend::Backend(BMV2::BMV2Options &options, P4::ReferenceMap *refMap, P4::TypeMap *typeMap, P4::ConvertEnums::EnumMapping *enumMap) {
    this->options = options;
    this->refMap = refMap;
    this->typeMap = typeMap;
    this->enumMap = enumMap;
}


native_object *Backend::process(const IR::ToplevelBlock *toplevel) {
    llvm::LLVMContext context;
    auto module = new llvm::Module("p4", context);

    std::cout << "name: " <<  toplevel->getName() << std::endl;
    std::cout << "name: " <<  toplevel->getMain()->getName() << std::endl;

    BMV2::PsaProgramStructure structure(this->refMap, this->typeMap);
    auto parsePsaArch = new BMV2::ParsePsaArchitecture(&structure);
    auto main = toplevel->getMain();
    if (!main || main->type->name != "PSA_Switch") {
        return nullptr;
    }
    main->apply(*parsePsaArch);

    std::cout << "headers size: " << structure.headers.size() << std::endl;
    std::cout << "actions size: " << structure.actions.size() << std::endl;
    std::cout << "globals size: " << structure.globals.size() << std::endl;

    // debug
    for (auto o : structure.headers) {
        std::cout << "test" << o.first << std::endl;
    }

    /**
     * package PSA_Switch<IH, IM, EH, EM, NM, CI2EM, CE2EM, RESUBM, RECIRCM> (
     *   IngressPipeline<IH, IM, NM, CI2EM, RESUBM, RECIRCM> ingress,
     *   PacketReplicationEngine pre,
     *   EgressPipeline<EH, EM, NM, CI2EM, CE2EM, RECIRCM> egress,
     *   BufferingQueueingEngine bqe);
     */
    /* auto packagePsaSwitch = toplevel->getMain(); */
    /* if (!packagePsaSwitch || packagePsaSwitch->type->name != "PSA_Switch") { */
    /*     return nullptr; */
    /* } */



    for (auto cv : main->constantValue) {
        if (cv.second->is<IR::Block>()) {
            std::cout << "BlockType " <<  cv.second->getNode()->node_type_name() << std::endl;;
            /* std::cout << "BlockString " <<  cv.second->getNode()->toString() << std::endl;; */

        }
    }
    return nullptr;
}

}
