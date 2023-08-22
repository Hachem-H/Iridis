-- This is a temporary script, will eventually be replaced with tree sitter

local keywords = {
    'if', 'else', 'for', 'while', 'match', 'return',
    'struct', 'proc', 'mod', 'enum',
    'defer','do', 'in', 'comptime', 'as', 'try',
    'extern', 'using',
}

local types = {
    'u8', 'u16', 'u32', 'u64',
    'i8', 'i16', 'i32', 'i64',
    'c8', 'c16', 'c32', 'c64',

    'f32', 'f64',

    'char', 'string', 'cstring',
}

local operators = {
    '+', '-', '*', '/', '%',
    ':', '=', '^', '&',
}

local booleans = {
    'true', 'false'
}

function GenerateVimScript()
    local syntax = [[
if exists("b:current_syntax")
    finish
endif

syntax keyword iridisKeywords ]] .. table.concat(keywords, ' ') .. [[

syntax keyword iridisTypes ]] .. table.concat(types, ' ') .. [[

syntax match iridisInteger "\-\?\<\d\+\>" display
syntax match iridisFloat "\-\?\<[0-9][0-9_]*\%(\.[0-9][0-9_]*\)\%([eE][+-]\=[0-9_]\+\)\=" display
syntax keyword iridisBool ]] .. table.concat(booleans, ' ') [[

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
hi link iridisBool     Boolean
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
    ]]
    return syntax
end

function GenerateVSCodeScript()
    local syntax = [[
{
    "scopeName": "source.iridis",
    "patterns": [
        {
            "name": "keyword.iridis",
            "match": "\\b(]] .. table.concat(keywords, "|") .. [[)\\b"
        },
        {
            "name": "type.iridis",
            "match": "\\b(]].. table.concat(types, "|") ..[[)\\b",
    
            "captures": {
                "0": { "name": "support.type.iridis" }
            }
        },
        {
            "name": "operator.iridis",
            "match": "[]]..table.concat(operators, "")..[[]",
            "captures": {
                "0": { "name": "keyword.operator.iridis" }
            }
        },
        {
            "name": "comment.iridis",
            "begin": "//",
            "end": "$",
            "patterns": [
                {
                    "match": ".+",
                    "name": "comment.line.iridis"
                }
            ]
        },
        {
            "name": "function.definition.iridis",
            "match": "\\b([a-zA-Z_][a-zA-Z0-9_]*)\\s*::(?=\\s*proc\\b)",
            "captures": {
                "1": { "name": "entity.name.function.iridis" }
            }
        },
        {
            "name": "number.iridis",
            "match": "\\b(\\d*\\.?\\d+)\\b",
            "captures": {
                "1": { "name": "constant.numeric.iridis" }
            }
        },
        {
            "name": "string.iridis",
            "begin": "\"",
            "end": "\"",
            "patterns": [
                {
                    "match": "\\\\(?:.|\\n)",
                    "name": "constant.character.escape.iridis"
                }
            ]
        },
        {
            "name": "boolean.true",
            "match": "true",
        },
        {
            "name": "boolean.false",
            "match": "false",
        },
        {
            "name": "function.call.iridis",
            "match": "\\b[a-zA-Z_][a-zA-Z0-9_]*\\s*(?=\\()",
            "captures": {
                "0": { "name": "entity.name.function.iridis" }
            }
        },
        {
            "name": "builtin.call.iridis",
            "match": "\\b[a-zA-Z_][a-zA-Z0-9_]*!\\s*(?=\\()",
            "captures": {
                "0": { "name": "support.function.builtin.iridis" }
            }
        },
        {
            "name": "comment.block.iridis",
            "begin": "\\/\\*",
            "end": "\\*\\/",
            "captures": {
                "0": { "name": "punctuation.definition.comment.iridis" }
            }
        }
    ]
}
    ]]
    return syntax
end

function GenerateSublimeSyntax()
    local syntax = [[
%YAML 1.2
---
name: Iridis
scope: source.iridis
file_extensions:
  - iridis
contexts:
  main:
    - include: global

  global:
    - include: comments
    - include: strings
    - include: string-escaped-char

    - match: \b(]]..table.concat(keywords, "|")..[[)\b
      scope: keyword.iridis
    
    - match: \b(]]..table.concat(types, "|")..[[)\b
      scope: type.iridis
      captures:
        0: support.type.iridis
    
    - match: \[]]..table.concat(operators, "")..[[]
      captures:
        0: keyword.operator.iridis

    - match: true
      scope: boolean.true
    - match: false
      scope: boolean.false

    - match: \b(\d*\.?\d+)\b
      captures:
        1: constant.numeric.iridis

    - match: \b([a-zA-Z_][a-zA-Z0-9_]*)\\s*::(?=\\s*proc\b)
      scope: function.definition.iridis
      captures:
        1: entity.name.function.iridis

    - match: \b[a-zA-Z_][a-zA-Z0-9_]*\s*(?=\()
      scope: function.call.iridis
      captures:
        0: entity.name.function.iridis

    - match: \b[a-zA-Z_][a-zA-Z0-9_]*!\s*(?=\()
      scope: builtin.call.iridis
      captures:
        0: support.function.builtin.iridis
        
# TODO(Hachem): Match them with the VSCode extension
  block-comment:
    - match: /\*
      scope: punctuation.definition.comment.iridis
      push:
        - meta_scope: comment.block.iridis
        - match: \*/
          scope: punctuation.definition.comment.iridis
          pop: true
        - include: block-comment
  comments:
    - include: block-comment
    - match: //
      scope: punctuation.definition.comment.iridis
      push:
        - meta_scope: comment.line.double-slash.iridis
        - match: \n
          pop: true
    - match: "#!"
      scope: punctuation.definition.comment.iridis
      push:
        - meta_scope: comment.line.double-slash.iridis
        - match: \n
          pop: true
  strings:
    - match: '"'
      scope: punctuation.definition.string.begin.iridis
      push:
        - meta_scope: string.quoted.double.iridis
        - match: '"'
          scope: punctuation.definition.string.end.iridis
          pop: true
        - include: string-escaped-char
    - match: "`"
      scope: punctuation.definition.string.begin.iridis
      push:
        - meta_scope: string.quoted.raw.iridis
        - match: "`"
          scope: punctuation.definition.string.end.iridis
          pop: true
    - match: "'"
      scope: punctuation.definition.string.begin.iridis
      push:
        - meta_scope: string.quoted.raw.iridis
        - match: "'"
          scope: punctuation.definition.string.end.iridis
          pop: true
        - include: string-escaped-char

  string-escaped-char:
    - match: '\\(\\|[abefnrutv''"]|x\h{2}|u\h{4}|U\h{8}|[0-7]{3})'
      scope: constant.character.escape.iridis
    - match: \\.
      scope: invalid.illegal.unknown-escape.iridis
    ]]
    return syntax
end

if arg[1] == 'vim' then
    print(GenerateVimScript())
elseif arg[1] == 'sublime' then
    print(GenerateSublimeSyntax())
elseif arg[1] == 'vscode' then
    print(GenerateVSCodeScript())
end
