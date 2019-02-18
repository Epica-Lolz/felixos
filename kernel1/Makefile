OBJECTS = boot.o kernel.o
CXX = i686-elf-g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
AS = i686-elf-as

LINKER = linker.ld

LDFLAGS = -T $(LINKER) -ffreestanding -O2 -nostdlib

felix.bin: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ -lgcc

%.o: %.cc
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o: %.S
	$(AS) -c $^ -o $@

clean:
	rm -rf *.o *.bin 
