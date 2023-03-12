#![no_main]


#[no_mangle]
pub fn parse(parser_args_ptr: i64) -> bool {
    if parser_args_ptr == 4 {
        true
    } else {
        false
    }
}
