add_repositories("self-repo https://github.com/wolfired/xmake-repo.git")
add_requires("minifb", "lodepng-c")

target("main")
    set_kind("binary")
    add_includedirs("./src")
    add_files("./src/*.c")
    add_packages("minifb", "lodepng-c")
