CXX = g++
CXXFLAGS = -std=c++17 -O2 -Isrc
LDFLAGS = -lGL -lglfw -lpthread -ldl

# OS detection
ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin)
        # macOS
        CXXFLAGS += -I/opt/homebrew/include -I/usr/local/include
        LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
        # Linux/Unix
        LDFLAGS = -lGL -lglfw -lpthread -ldl
    endif
else
    # Windows (MinGW/MSYS)
    # Assuming GLFW is installed and in path, or linked statically
    LDFLAGS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32
    TARGET_EXT = .exe
endif

SRCDIR = src
OBJDIR = build

SRCS = $(SRCDIR)/Emulator.cpp \
       $(SRCDIR)/Bus.cpp \
       $(SRCDIR)/Cartridge.cpp \
       $(SRCDIR)/Mapper.cpp \
       $(SRCDIR)/Mapper0.cpp \
       $(SRCDIR)/nesCPU.cpp \
       $(SRCDIR)/nesPPU.cpp \
       $(SRCDIR)/Renderer.cpp \
       $(SRCDIR)/nesAPU.cpp

OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

TARGET = nes_emulator$(TARGET_EXT)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean