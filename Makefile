HIPCC=/usr/bin/hipcc

LDFLAGS :=
CXXFLAGS := -I/opt/rocm/include -std=c++17

HIP_PROGRAMS := average dump_buffer show_dumped_data show_dumped_data_pair show_data_pair do_product_from_file set_float_data show_dumped_tensor compare_dumped_data_pair check_bf16_data

all: $(HIP_PROGRAMS)

# Step

average: average.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

dump_buffer: dump_buffer.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

show_dumped_data: show_dumped_data.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

show_dumped_data_pair: show_dumped_data_pair.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

show_dumped_tensor: show_dumped_tensor.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

show_data_pair: show_data_pair.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

do_product_from_file: do_product_from_file.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

set_float_data: set_float_data.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

compare_dumped_data_pair: compare_dumped_data_pair.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

check_bf16_data: check_bf16_data.o
	$(HIPCC)  -o $@  $^   $(LDFLAGS) 

%.o: %.cpp
	$(HIPCC) $(CXXFLAGS) -c $^  -o $@ 

clean:
	rm -f *.o $(HIP_PROGRAMS)

clean_all:
	rm -f *.o $(HIP_PROGRAMS) *.bundle *.hsaco *.isa

