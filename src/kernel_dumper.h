// Copyright Hugh Perkins 2016

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// this takes in the module, and the name of a kernel, and does whatever we need to do to get some
// working opencl code out of it :-)

#include "llvm/IR/Module.h"

#include <string>
#include <set>

namespace cocl {

class KernelDumper {
public:
    KernelDumper(llvm::Module *M, std::string kernelName) :
        M(M), kernelName(kernelName) {

    }
    virtual ~KernelDumper() {}
    std::string toCl();

    llvm::Module *M;
    std::string kernelName;
    std::set<std::string> functionDeclarations;
    std::set<llvm::StructType *>structsToDefine;

protected:
};

} // namespace cocl
