## Требования к запуску

 - CMake v3.11
 - Libgtest-dev (GTest)

 

### Установка CMake
 - Удалить предыдущую версию
```bash
sudo apt remove cmake
sudo apt purge --auto-remove cmake
```

 - Установка CMake (10-15 минут)
```bash
wget https://cmake.org/files/v3.11/cmake-3.11.1.tar.gz
tar -xzvf cmake-3.11.1.tar.gz
cd cmake-3.11.1/
./bootstrap
make -j4
sudo make install
```

 - Очистка загрузок
```bash
cd ..
rm -rf cmake-3.11.1 cmake-3.11.1.tar.gz
```


### Установка GTest
```bash
sudo apt-get install cmake libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
```
