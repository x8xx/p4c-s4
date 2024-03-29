mod wasmer;

use std::ptr::null;
use std::slice::from_raw_parts;

#[repr(C)]
pub struct NativeObjectFromWasm {
   obj: *const u8, 
   size: usize,
}


#[no_mangle]
pub extern "C" fn wasm_to_native_object(wasm: *const u8, wasm_size: usize) -> NativeObjectFromWasm {
    unsafe {
        if wasm_size < 4 ||
            *wasm != 0x00 ||
            *wasm.offset(1) != 0x61 ||
            *wasm.offset(2) != 0x73 ||
            *wasm.offset(3) != 0x6d {
            return NativeObjectFromWasm {
                obj: null(),
                size: 0,
            };
        }
    }

    let native_object = wasmer::gen_native_object(unsafe { from_raw_parts(wasm, wasm_size) });
    
    NativeObjectFromWasm {
        obj: wasm,
        size: wasm_size,
    }
}
