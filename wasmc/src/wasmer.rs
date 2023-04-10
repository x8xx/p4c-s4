use std::fs::File;
use std::io::BufWriter;
use wasmer::{ Store, Target, Triple, CpuFeature, Artifact };
use wasmer_compiler_llvm::LLVM;
// use wasmer_compiler::Artifact;

pub type PrefixerFn = Box<dyn Fn(&[u8]) -> String + Send>;

pub fn gen_native_object(wasm: &[u8]) -> &[u8] {
    let llvm = LLVM::new();
    let store = Store::new(llvm);
    let engine = store.engine();
    let engine_inner = engine.inner();
    let compiler = engine_inner.compiler().unwrap();
    let features = engine_inner.features();
    let tunables = store.tunables();
    let prefixer: Option<PrefixerFn> = None;
    let target = Target::new(Triple::host(), CpuFeature::for_host());
    let compile_result = Artifact::generate_object(compiler, wasm, prefixer, &target, tunables, &features);
    if compile_result.is_ok() {
        let compile_result = compile_result.unwrap();
        let writer = BufWriter::new(File::create("test.o").unwrap());
        compile_result.1.write_stream(writer);
        println!("ok yazo {}", compile_result.2);
    } else {
        println!("ng yazo");
    }
    // Artifact::generate_object(&compiler, wasm, prefixer);
    // Artifact::generate_object
    wasm
}
