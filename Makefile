CXX	= /usr/bin/clang++

BUILDDIR = build
SOURCEDIR = src
IMGUI_DIR = lib/imgui

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_sdlrenderer2.cpp
OBJECTS = $(addprefix build/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CFLAGS  = -std=c++20 -g -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends `sdl2-config --cflags`
LDFLAGS = -lSDL2

NAME = game
BINARY = $(BUILDDIR)/$(NAME)

RM = rm -rf
MKDIR = mkdir

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINARY)

clean:
	$(RM) $(BINARY)
	$(RM) $(BUILDDIR)

run: $(BINARY)
	$(BINARY)
