# Calculator GRPC
A gRPC Linuxon készítve
# Protocol Buffers feltelepítése

A gRPC alapértelmezés szerint protokollpuffereket használ, a protoc fordítóra lesz szükséged a szerver és kliens kódok létrehozásához.
Ezt nem muszáj lebuildelni, az alábbi linken le is tölthetjük a protoc 22.3 futtatható változatát. 
A protoc bin mappájának - ahol a futtatható parancs található - elérési útvonalát érdemes a PATH környezeti változóhoz rendelni.
[Protoc 22.3](https://github.com/protocolbuffers/protobuf/releases/download/v22.3/protoc-22.3-win64.zip)

Válasszon egy könyvtárat a helyileg telepített dependencyknek. 
A MY_INSTALL_DIR környezeti változó tartalmazza a Protocol Buffers elérési útvonalát.
Ez a példában az aktuális user home könyvtárában (ezt a /home/user1 vagy $HOME vagy ~ formájában is jelezhetjük) 
Érdemes kitenni egy külön változóba, mert többször fogunk hivatkozni rá. Ez a változó csak az aktuális shell kontextusában kerül lefoglalásra.

```
$ export MY_INSTALL_DIR=$HOME/.local
$ mkdir -p $MY_INSTALL_DIR
```

Adjuk hozzá a Protobuf bin mappáját a PATH környezeti változóhoz, így:

```
$ export PATH="$MY_INSTALL_DIR/bin:$PATH"
```

A CMake telepítése
A CMake legalább 3.13-as verziójára van szükségünk

Például a 3.19.6-os verziót így telepíthetjük a helyi telepítési könyvtárába a következőképpen:

```
$ wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh
$ sh cmake-linux.sh -- --skip-license --prefix=$MY_INSTALL_DIR
$ rm cmake-linux.sh
```

Telepítse a gRPC elkészítéséhez szükséges alapvető eszközöket:

```
$ sudo apt install -y build-essential autoconf libtool pkg-config
```

Klónozzuk a gRPC repositoryt annak almoduljaival együtt:

```
$ git clone --recurse-submodules -b v1.54.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
```

A gRPC és a Protocol Buffers összeállítása és telepítése
Bár nem kötelező, de a gRPC alkalmazások általában kihasználják a Protocol Buffers-t a szolgáltatásdefiníciókhoz és az adatok szerializációjához, a példakód pedig a proto3-at használja.

A következő parancsok a gRPC-t és a Protocol Buffers-t építik és lokálisan telepítik:

```
$ cd grpc
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
$ make -j 4
$ make install
$ popd
```

Ezek után kezdhetjük a programok buildelését.

## A Calculator elkészítése így történt:

1. grpc examples mappájában elkészítünk egy calculator mappát
2. calculator.proto állományt elkészítjük a calculator mappán belül
3. Létrehozzuk a következő állományokat `helloworld` példa alapján:
   1. CMakeLists.txt
   2. calculator_client.cc
   3. calculator_server.cc
4. GRPC osztályok (message-ek) elkészítése:

```protoc --cpp_out=./cmake/build/ calculator.proto```

5. GRPC metódusok (service-ek) lebuildelése: 

```protoc --grpc_out=./cmake/build/ --plugin=protoc-gen-grpc=\`which grpc_cpp_plugin` calculator.proto ```

Ezek a parancsok a /cmake/build/ mappában hoznak létre .grpc.pb.cc .grpc.pb.h pb.cc pb.h állományokat

6. Bemegyünk a build mappába a `cd /cmake/build/` paranccsal
7. `cmake -DCMAKE_PREFIX_PATH=$PROTOBUF_INSTALL_DIR ../..` ahol 
`$PROTOBUF_INSTALL_DIR`: az a hely, ahová telepítetted a Protocol Bufferst

Innentől minden változtatásnál `make -j4` parancsot kell futtatni (a make -j kapcsolójának 4-es értéke azt jelenti, hogy 4 szálon futtatja a műveletet -> gyorsabb)
Ökölszabály: n = a CPU magjainak száma. 
Tehát ha kétmagos CPU-val dolgozol, használhatod a -j2-t, míg egy 8 magos gépen a -j8-at.

# Fejlesztői környezet
- IDE: CLion
- Oprendszer: Linux 22.04.2 LTS
- CMake (Makefileok előállítása dependencykkel + linkelésekkel) 3.19.6 verzió
- g++ (C++ Compiler) 11.3.0 verzió
- GNU Make (Makefile alapján buildelés) 4.3 verzió
- protoc (Proto Compiler) 3.21.12 verzió
- Protocol Buffers 3.21.12
- gRPC 1.54.0
