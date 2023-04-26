# Calculator GRPC
A gRPC Linuxon készítve
# Protocol Buffers telepítése
todo...

## A Calculator elkészítése így történt:

1. examples mappában elkészítünk egy calculator mappát
2. calculator.proto állományt elkészítésjük a calculator mappán belül
3. Elkészítjük a következő állományokat `helloworld` példa alapján:
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
