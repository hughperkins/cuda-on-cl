#include "type_dumper.h"

#include "EasyCL/util/easycl_stringhelper.h"

#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
// #include "llvm/IR/Type.h"
// #include "llvm/IR/FunctionType.h"

#include <iostream>

using namespace std;
using namespace llvm;
using namespace cocl;

namespace cocl {

std::string TypeDumper::dumpAddressSpace(llvm::Type *type) {
    if(PointerType *ptr = dyn_cast<PointerType>(type)) {
        int addressspace = ptr->getAddressSpace();
        switch(addressspace) {
            case 0:
                return "";
            case 1:
                return "global";
            case 3:
                return "local";
            default:
                throw runtime_error("not implemented, addressspace " + easycl::toString(addressspace));
        }
    } else {
        return "";
    }
}

std::string TypeDumper::dumpPointerType(PointerType *ptr) {
    string gencode = "";
    Type *elementType = ptr->getPointerElementType();
    string elementTypeString = dumpType(elementType);
    int addressspace = ptr->getAddressSpace();
    string addressspacestr = "";
    if(addressspace == 1) {
        addressspacestr = "global";
    }
    if(addressspace == 3) {
        addressspacestr = "local";
    }
    if(addressspace == 4) {
        addressspacestr = "constant";
    }
    // we're just going to hackily assume that anything that is `global **` should be `global * global *`
    // if(isa<PointerType>(ptr->getPointerElementType())) {
        // return "global " + elementTypeString + addressspacestr + " *";
        // return elementTypeString + addressspacestr + " *";
        // return "global " + elementTypeString + addressspacestr + " *";
    // }
    if(addressspacestr != "") {
        gencode += addressspacestr + " ";
    }
    gencode += elementTypeString + "*";
    return gencode;
}

std::string TypeDumper::dumpIntegerType(IntegerType *type) {
    switch(type->getPrimitiveSizeInBits()) {
        case 32:
            return "int";
        case 64:
            return "long";
        case 16:
            return "short";
        case 8:
            return "char";
        case 1:
            return "bool";
        default:
            outs() << "integer size " << type->getPrimitiveSizeInBits() << "\n";
            throw runtime_error("unrecognized size");
    }
}

std::string TypeDumper::dumpStructType(StructType *type) {
    // outs() << "dumpstructtype" << "\n";
    if(type->hasName()) {
        string name = type->getName();
        // outs() << "name " << name << "\n";
        name = easycl::replace(name, ".", "_");
                name = easycl::replace(name, ":", "_");
        if(name == "struct_float4") {
            return "float4";
        } else {
            if(name.find("struct_") == 0) {
                name[6] = ' ';
                structsToDefine[type] = name;
                // if(declaredStructs.find(name) == declaredStructs.end()) {
                //     declaredStructs.insert(name);
                //     declareStruct(name, type);
                // }
                return name;
            } else if(name.find("class_") != string::npos) {
                // name[5] = '_';
                name = "struct " + name;
                structsToDefine[type] = name;
                // if(declaredStructs.find(name) == declaredStructs.end()) {
                //     declaredStructs.insert(name);
                //     declareStruct(name, type);
                // }
                return name;
            } else {
                name = "struct " + name;
                structsToDefine[type] = name;
                // if(declaredStructs.find(name) == declaredStructs.end()) {
                //     declaredStructs.insert(name);
                //     declareStruct(name, type);
                // }
                return name;
                // outs() << "struct name: " << name << "\n";
                // throw runtime_error("dumpStructType() not implemented: struct name " + name);
            }
        }
    } else {
        type->dump();
        throw runtime_error("not implemented: anonymous struct types");
    }
}

std::string TypeDumper::dumpArrayType(ArrayType *type) {
    int length = type->getNumElements();
    Type *elementType = type->getElementType();
    return dumpType(elementType) + "[" + easycl::toString(length) + "]";
}

std::string TypeDumper::dumpFunctionType(FunctionType *fn) {
    throw runtime_error("not implemented");
    // // outs() << "function" << "\n";
    // std::string params_str = "";
    // int i = 0;
    // for(auto it=fn->param_begin(); it != fn->param_end(); it++) {
    //     Type * paramType = *it;
    //     if(i > 0) {
    //         params_str += ", ";
    //     }
    //     params_str += dumpType(paramType);
    //     i++;
    // }
    // // outs() << "params_str " << params_str << "\n";
    // return params_str;
}

std::string TypeDumper::dumpType(Type *type) {
    Type::TypeID typeID = type->getTypeID();
    switch(typeID) {
        case Type::VoidTyID:
            return "void";
        case Type::FloatTyID:
            return "float";
        // case Type::UnionTyID:
        //     throw runtime_error("not implemented: union type");
        case Type::StructTyID:
            return dumpStructType(cast<StructType>(type));
        case Type::VectorTyID:
            throw runtime_error("not implemented: vector type");
        case Type::ArrayTyID:
            return dumpArrayType(cast<ArrayType>(type));
        case Type::DoubleTyID:
            // if(single_precision) {
                // return "float";
            // } else {
                return "double";
            // }
        case Type::FunctionTyID:
            return dumpFunctionType(cast<FunctionType>(type));
        case Type::PointerTyID:
            return dumpPointerType(cast<PointerType>(type));
        case Type::IntegerTyID:
            return dumpIntegerType(cast<IntegerType>(type));
        default:
            outs() << "type id " << typeID << "\n";
            throw runtime_error("unrecognized type");
    }
}


} // namespace cocl;
