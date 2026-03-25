env = Environment(
    CCFLAGS=['-std=c++20', '-Wall', '-O0', '-g']
)

env.ParseConfig('pkg-config --cflags --libs opencv4')

env.Program('ascimg.cpp')
