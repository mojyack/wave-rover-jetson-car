# build python for jetson
the controller uses some newer features of python, which may be not available on default l4t.  
so it is recommended to build latest python from source.
```
# install dependencies
sudo apt install libffi-dev libssl-dev
# download and unpack
ver=3.11.8
curl -OL 'https://www.python.org/ftp/python/3.11.8/Python-$ver.tgz'
tar -xvf Python-$ver.tgz
cd Python-$ver
# configure
./configure --prefix=$HOME/.local --enable-optimizations
# build
make -j$(nproc)
# install
make install
```
now python installed to `$HOME/.local/bin`
