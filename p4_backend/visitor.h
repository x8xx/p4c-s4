#include "ir/pass_manager.h""
#include "frontends/p4/typeMap.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "backends/bmv2/psa_switch/psaProgramStructure.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>


namespace S4 {
class S4Visitor : public Inspector {
    private:
        P4::ReferenceMap *refMap;
        P4::TypeMap *typeMap;
        BMV2::PsaProgramStructure *pinfo;

        llvm::Module *module;
        llvm::Function *currentFunction;

    public:
        S4Visitor(P4::ReferenceMap *refMap, P4::TypeMap *typeMap, BMV2::PsaProgramStructure *pinfo, llvm::Module *module)
            : refMap(refMap), typeMap(typeMap), pinfo(pinfo), module(module) {
            CHECK_NULL(refMap);
            CHECK_NULL(typeMap);
            CHECK_NULL(pinfo);
            CHECK_NULL(module);
            setName("InspectPsaProgram");
        }


        bool preorder(const IR::Node*) override;
        /* void postorder(const IR::Node*) override; */

        bool preorder(const IR::P4Program*) override;
        bool preorder(const IR::P4Parser*) override;

        /* bool preorder(const IR::Type_Error*) override; */
        /* bool preorder(const IR::Declaration_ID*) override; */
        /* bool preorder(const IR::Type_Extern*) override; */


        /* bool preorder(const IR::Expression*) override; */
        /* void postorder(const IR::Expression*) override; */

};
}
