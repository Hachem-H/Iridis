if exists("b:current_syntax")
    finish
endif

syntax keyword iridisKeywords if else for while match return struct proc mod enum defer do in comptime as try extern using
syntax keyword iridisTypes u8 u16 u32 u64 i8 i16 i32 i64 c8 c16 c32 c64 f32 f64 char string cstring
syntax match iridisInteger "\-\?\<\d\+\>" display
syntax match iridisFloat "\-\?\<[0-9][0-9_]*\%(\.[0-9][0-9_]*\)\%([eE][+-]\=[0-9_]\+\)\=" display
syntax match iridisHex "\<0[xX][0-9A-Fa-f]\+\>" display
syntax match iridisDoz "\<0[zZ][0-9a-bA-B]\+\>" display
syntax match iridisOct "\<0[oO][0-7]\+\>" display
syntax match iridisBin "\<0[bB][01]\+\>" display

syntax region iridisRawString start=+`+ end=+`+
syntax region iridisChar start=+'+ skip=+\\\\\|\\'+ end=+'+
syntax region iridisString start=+"+ skip=+\\\\\|\\'+ end=+"+ contains=iridisEscape
syntax match iridisEscape display contained /\\\([nrt\\'"]\|x\x\{2}\)/

syntax region iridisLineComment start=/\/\// end=/$/
syntax region iridisBlockComment start=/\v\/\*/ end=/\v\*\//

syntax match iridisFunctionDecl "\v<\w*>(\s*::\s*proc)@="
syntax match iridisFunctionCall "\v\w+\s*(\()@="
syntax match iridisBuiltinCall "\v\w+!\s*(\()@="

syntax match iridisPointer "\^" display
syntax match iridisAddress "&"  display
syntax match iridisSet ":"  display
syntax match iridisEql "="  display
syntax match iridisMod "%"  display
syntax match iridisAdd "+"  display
syntax match iridisSub "-"  display
syntax match iridisMul "*"  display
syntax match iridisDiv "/"  display

hi link iridisFunctionDecl Function
hi link iridisFunctionCall Function
hi link iridisBuiltinCall  Macro

hi link iridisLineComment  Comment
hi link iridisBlockComment Comment

hi link iridisKeywords Keyword
hi link iridisTypes    Type

hi link iridisPointer Operator
hi link iridisAddress Operator

hi link iridisRawString String
hi link iridisChar      String
hi link iridisString    String

hi link iridisInteger Number
hi link iridisFloat   Number
hi link iridisHex     Number
hi link iridisDoz     Number
hi link iridisOct     Number
hi link iridisBin     Number

let b:current_syntax = "iridis"
    
