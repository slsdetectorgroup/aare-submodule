# aare-submodule

```bash
#Clone the examples and the submodule
git clone https://github.com/slsdetectorgroup/aare-submodule.git --recursive
cd aare-submodule
mkdir build
cd build

cmake .. 

#or if you dont want to build the python ext
# cmake .. -DAARE_PYTHON_BINDINGS=OFF 

make -j4
./00-NDArray

```