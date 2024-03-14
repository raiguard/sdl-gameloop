CXX	= /usr/bin/clang++

BUILDDIR = build
SOURCEDIR = src
IMGUI_DIR = lib/imgui

GLAD = lib/glad/src/glad.c
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(GLAD)
OBJECTS = $(addprefix build/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CFLAGS  = -std=c++20 -g -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -Ilib/glad/include `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

NAME = game
BINARY = $(BUILDDIR)/$(NAME)

RM = rm -rf
MKDIR = mkdir

all: $(BUILDDIR)/ $(BINARY)

$(GLAD):
	glad --out-path lib/glad --generator c

$(BUILDDIR)/:
	$(MKDIR) -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: lib/glad/src/glad.c
	$(CXX) $(CFLAGS) -c $< -o $@

$(BINARY):  $(GLAD) $(OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINARY)

clean:
	$(RM) $(BINARY)
	$(RM) $(BUILDDIR)

run: $(BINARY)
	$(BINARY)
