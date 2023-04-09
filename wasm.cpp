#include <iostream>
#include <fstream>
#include "wasm.h"
#include "wasmc/wasmc.h"


namespace S4 {

native_object *wasmCompile(const char* wasmPath) {
    std::ifstream fin(wasmPath, std::ios::in | std::ios::binary); 

    if (!fin) {
        std::cout << "error: cant open wasm binary - " << wasmPath << std::endl;
        std::exit(1);
    }

    fin.seekg(0, std::ios::end);
    std::streampos wasmSize = fin.tellg();
    fin.seekg(0);

    uint8_t* wasm = new uint8_t[wasmSize];
    if (!fin.eof()) {
        std::cout << "debug" << std::endl;
        fin.read(reinterpret_cast<char*>(wasm), wasmSize);
    }

    fin.close();

    NativeObjectFromWasm nativeObjectFromWasm = wasm_to_native_object(wasm, wasmSize);
    if (!nativeObjectFromWasm.obj) {
        std::cout << "error: cant loading wasm binary - " << wasmPath << std::endl;
        std::exit(1);
    }

    return new native_object {
        nativeObjectFromWasm.obj,
        nativeObjectFromWasm.size,
    };
}
}
