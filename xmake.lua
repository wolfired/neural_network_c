add_repositories("self-repo https://github.com/wolfired/xmake-repo.git")
add_requires("minifb", "lodepng-c")

target("matrix")
    set_kind("static")
    add_includedirs("./src")
    add_files("./src/matrix.c")

target("bresenham")
    set_kind("static")
    add_includedirs("./src")
    add_files("./src/bresenham.c")

target("canvas")
    set_kind("binary")
    add_files("./src/canvas.c")
    add_packages("minifb", "lodepng-c")
    add_deps("matrix")
    add_deps("bresenham")
    set_default(false)

target("main")
    set_kind("binary")
    add_files("./src/main.c")
    add_deps("matrix")
    set_default(true)
