CXXFLAGS = -shared -fPIC --no-gnu-unique -g -std=c++2b -Wno-c++11-narrowing
INCLUDES = `pkg-config --cflags pixman-1 libdrm hyprland pangocairo libinput libudev wayland-server xkbcommon`
LIBS = `pkg-config --libs pangocairo`

SRC = main.cpp ToplevelUtils.cpp
TARGET = hypruntile.so
LOAD_TARGET = $(abspath $(TARGET))

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $> -o $@ $(LIBS) -O2

clean:
	rm ./$(TARGET)

load: all unload
	hyprctl plugin load $(LOAD_TARGET)

unload:
	hyprctl plugin unload $(LOAD_TARGET)

.PHONY: all clean load unload
