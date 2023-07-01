#include "visitor.h""
#include <iostream>

namespace S4 {
/**
 * Node
 */
bool S4Visitor::preorder(const IR::Node *node) {
    std::cout << "1_: " << node->toString() << " : " << node->node_type_name() << std::endl;
    return true;
}
/* void S4Visitor::postorder(const IR::Node *node) { */
/*     std::cout << "2_: "  << node->toString() <<  " : " << node->node_type_name() << std::endl; */
/* } */


/**
 * P4Program
 */
bool S4Visitor::preorder(const IR::P4Program *node) {
    std::cout << "preorder-p4-program: " << node->toString() << " : " << node->node_type_name() << std::endl;
    return true;
}


/**
 * P4Parser
 */
bool S4Visitor::preorder(const IR::P4Parser *node) {
    std::cout << "preorder-p4-parser: " << node->toString() << " : " << node->node_type_name() << std::endl;

    std::vector<llvm::Type*> parserFunctionArgs;
    parserFunctionArgs.push_back(llvm::Type::getInt8PtrTy(this->module->getContext()));
    parserFunctionArgs.push_back(llvm::Type::getInt16Ty(this->module->getContext()));
    parserFunctionArgs.push_back(llvm::Type::getInt64PtrTy(this->module->getContext()));
    llvm::FunctionType *parserFT = llvm::FunctionType::get(llvm::Type::getVoidTy(this->module->getContext()), parserFunctionArgs, false);
    this->currentFunction = llvm::Function::Create(parserFT, llvm::Function::ExternalLinkage, "parser", this->module);

    /* this->module->getFunctionList().push_back(); */
    return true;
}





/*/1** */
/* * Type_Error */
/* *1/ */
/*bool S4Visitor::preorder(const IR::Type_Error *node) { */
/*    std::cout << "preorder-type-error: " << node->toString() << std::endl; */
/*    return true; */
/*} */


/*/1** */
/* * Declaration_ID */
/* *1/ */
/*bool S4Visitor::preorder(const IR::Declaration_ID *node) { */
/*    std::cout << "preorder-DeclarationID: " << node->getName().toString() << std::endl; */
/*    return false; */
/*} */


/*/1** */
/* * Type_Extern */
/* *1/ */
/*bool S4Visitor::preorder(const IR::Type_Extern *node) { */
/*    std::cout << "preorder-type-extern: " << node->toString() << std::endl; */
/*    return true; */
/*} */



/*bool S4Visitor::preorder(const IR::Expression *node) { */
/*    std::cout << "preorder-expression: " << node->toString() << std::endl; */
/*    return true; */
/*} */
/*void S4Visitor::postorder(const IR::Expression *node) { */
/*    std::cout << "postorder-expression: "  << node->toString() << std::endl; */
/*} */
}
