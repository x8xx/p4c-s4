#include "backend.h"
#include "visitor.h""

namespace S4 {

Backend::Backend(BMV2::BMV2Options &options, P4::ReferenceMap *refMap, P4::TypeMap *typeMap, P4::ConvertEnums::EnumMapping *enumMap) {
    this->options = options;
    this->refMap = refMap;
    this->typeMap = typeMap;
    this->enumMap = enumMap;
}


native_object *Backend::process(const IR::ToplevelBlock *toplevel) {
    /**
     * init LLVM
     */
    llvm::LLVMContext context;
    auto module = new llvm::Module("p4", context);

    /* std::cout << "name: " <<  toplevel->getName() << std::endl; */
    /* std::cout << "name: " <<  toplevel->getMain()->getName() << std::endl; */




    /**
     * P4 IR -> LLVM IR
     */
    BMV2::PsaProgramStructure structure(this->refMap, this->typeMap);
    auto parsePsaArch = new BMV2::ParsePsaArchitecture(&structure);
    auto main = toplevel->getMain();
    if (!main || main->type->name != "PSA_Switch") {
        return nullptr;
    }

    // backends/bmv2/psa_switch/psaProgramStructure.cpp
    // bool ParsePsaArchitecture::preorder(const IR::PackageBlock *block)
    main->apply(*parsePsaArch);

    auto evaluator = new P4::EvaluatorPass(this->refMap, this->typeMap);
    PassManager pass = {
        evaluator,
        new S4Visitor(this->refMap, this->typeMap, &structure, module),
        /* new VisitFunctor( */
        /*     [this, evaluator, structure]() { toplevel = evaluator->getToplevelBlock(); }), */
        /* [this, evaluator, structure]() { auto tlb = evaluator->getToplevelBlock(); }, */
        /* [this, evaluator, structure]() { std::cout << "pass debug" <<  evaluator->getToplevelBlock()->getName() << std::endl; }, */
    };

    /* /1* auto hook = options.getDebugHook(); *1/ */
    /* /1* rewriteToEBPF.addDebugHook(hook, true); *1/ */
    auto program = toplevel->getProgram();
    program = program->apply(pass);




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
