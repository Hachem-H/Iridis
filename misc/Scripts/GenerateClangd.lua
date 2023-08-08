local file = io.open("meson.build", "r")
if not file then
    print("Error: Unable to open 'meson.build' file.")
    return
end

local content = file:read("*all")
file:close()

local includeDirectories = {}
for includeDir in content:gmatch("include_directories%('([^']+)'%)") do
    table.insert(includeDirectories, includeDir)
end

local compileFlags = "-Wall\n-Wextra\n"
for _, includeDir in ipairs(includeDirectories) do
    compileFlags = compileFlags .. "-I" .. includeDir .. "\n"
end

local outputFile = io.open("compile_flags.txt", "w")

if not outputFile then
    print("Error: Unable to create 'compile_flags.txt' file.")
    return
end

outputFile:write(compileFlags)
outputFile:close()

print("'compile_flags.txt' was successfully generated.")
