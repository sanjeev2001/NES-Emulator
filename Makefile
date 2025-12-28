CXX = g++
CXXFLAGS = -std=c++17 -O2 -Isrc
LDFLAGS = -lGL -lglfw -lpthread -ldl

# OS detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	# macOS
	CXXFLAGS += -I/opt/homebrew/include -I/usr/local/include
	LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
	# Linux/Unix
	LDFLAGS = -lGL -lglfw -lpthread -ldl
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

TARGET = nes_emulator

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
