INCLUDE_DIR := realm-core/src
OBJ := realm-core/build.DEBUG/src/realm
OPENSSL := /usr/local/opt/openssl/lib/**.a
CXXFLAGS=-std=c++17

realm: main.cpp
	g++ $(CXXFLAGS) -I $(INCLUDE_DIR) -framework Foundation -framework Security -lz main.cpp $(OBJ)/librealm-dbg.a $(OBJ)/sync/librealm-server-dbg.a $(OBJ)/object-store/librealm-object-store-dbg.a $(OBJ)/sync/librealm-sync-dbg.a 

