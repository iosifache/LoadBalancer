# Flags for compiler and linker
CPP = g++
CPP_FLAGS = -g
SCITER_LINK_FLAGS = `pkg-config --cflags --libs gtk+-3.0` -ldl
MULTI_THREATHING_LINK_FLAGS = -lpthread -pthread

# Project's components
COMMON_BUILD = common
SCITER_BUILD = sciter

# Project's folders
UI_FOLDER = ui
HEADER_FOLDER = headers
SOURCE_FOLDER = sources
BUILD_FOLDER = build
COMMON_HEADER_FOLDER = $(HEADER_FOLDER)/$(COMMON_BUILD)
SCITER_HEADER_FOLDER = $(HEADER_FOLDER)/$(SCITER_BUILD)
COMMON_SOURCE_FOLDER = $(SOURCE_FOLDER)/$(COMMON_BUILD)
SCITER_SOURCE_FOLDER = $(SOURCE_FOLDER)/$(SCITER_BUILD)

# Project's files
COMMON_SOURCE_FILES = $(COMMON_SOURCE_FOLDER)/*
SCITER_SOURCE_FILES = $(SCITER_SOURCE_FOLDER)/*

# Sciter's folder and files
SCITER_SDK_FOLDER = ./libraries/sciter-sdk
SCITER_INCLUDE_FOLDER = $(SCITER_SDK_FOLDER)/include
SCITER_BINARIES_FOLDER = $(SCITER_SDK_FOLDER)/bin.lnx
SCITER_LIBRARIES_FOLDER = $(SCITER_BINARIES_FOLDER)/x64
SCITER_UI_PACKER = $(SCITER_BINARIES_FOLDER)/packfolder

# Sciter's files
SCITER_GTK_MAIN_SOURCE_FILE = $(SCITER_INCLUDE_FOLDER)/sciter-gtk-main.cpp
SCITER_MAIN_LIBRARY = $(SCITER_LIBRARIES_FOLDER)/libsciter-gtk.so

# Build source files
SERVER_SOURCE_FILES = $(COMMON_SOURCE_FOLDER)/CPacket.cpp $(COMMON_SOURCE_FOLDER)/CServer.cpp $(COMMON_SOURCE_FOLDER)/MainServer.cpp
LOAD_BALANCER_SOURCE_FILES = $(COMMON_SOURCE_FOLDER)/CPacket.cpp $(COMMON_SOURCE_FOLDER)/CServer.cpp $(COMMON_SOURCE_FOLDER)/CLoadBalancer.cpp $(SCITER_SOURCE_FILES)

# Executables filenames
LOAD_BALANCER_EXECUTABLE_FILE = $(BUILD_FOLDER)/load_balancer
SERVER_EXECUTABLE_FILE = $(BUILD_FOLDER)/server

# Special attributes for targets
.PHONY: run
.ONESHELL: run

# Target for running all build targets
all: clean build_load_balancer build_server

# Target for running the load balancer
run:
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(SCITER_LIBRARIES_FOLDER)
	./$(LOAD_BALANCER_EXECUTABLE_FILE)

# Target for building the load balancer
build_load_balancer:
	chmod +x $(SCITER_UI_PACKER)
	$(SCITER_UI_PACKER) $(UI_FOLDER) $(SCITER_SOURCE_FOLDER)/Resources.cpp -v "resources"
	$(CPP) $(CPP_FLAGS) $(LOAD_BALANCER_SOURCE_FILES) $(SCITER_GTK_MAIN_SOURCE_FILE) -I $(SCITER_INCLUDE_FOLDER) -I $(COMMON_HEADER_FOLDER) -I $(SCITER_HEADER_FOLDER) -L $(SCITER_LIBRARIES_FOLDER) $(SCITER_MAIN_LIBRARY) $(SCITER_LINK_FLAGS) -o $(LOAD_BALANCER_EXECUTABLE_FILE)

# Target for building the servers
build_server:
	$(CPP) $(CPP_FLAGS) $(SERVER_SOURCE_FILES) -I $(COMMON_HEADER_FOLDER) $(MULTI_THREATHING_LINK_FLAGS) -o $(SERVER_EXECUTABLE_FILE)

# Target for cleaning the project
clean:
	rm -f $(SCITER_SOURCE_FOLDER)/Resources.cpp
	rm -rf $(BUILD_FOLDER)/*