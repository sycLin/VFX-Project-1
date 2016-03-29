alignAll: align1 align2

align1: a.out
	@echo "performing image alignment on input/scene1/"
	@./a.out input/scene1 detail.txt output/scene1/aligned

align2: a.out
	@echo "performing image alignment on input/scene2/"
	@./a.out input/scene2 expose.txt output/scene2/aligned

a.out: main.cpp
	@echo "compiling the c++ source file..."
	@g++ main.cpp `pkg-config --libs --cflags opencv` -o a.out
	@echo "compiled into executable: a.out"

clean:
	@echo "removing executable: a.out"
	rm -rf a.out
	@echo "removing aligned images"
	rm -rf output/scene1/aligned/*
	rm -rf output/scene2/aligned/*

HDRAll: HDR1 HDR2

HDR1:
	@echo "performing HDR imaging on input/scene1/"
	@cd matlab
	@matlab -nodisplay -nosplash -nodesktop -r 'hw1("../input/scene1/", "", "1", ".JPG", "../input/scene1/detail.txt");'
	@cd ..
	@echo "performing HDR imaging on output/scene1/aligned/"
	
HDR2:
	@echo "performing HDR imaging on input/scene2/"
	@cd matlab
	@matlab -nodisplay -nosplash -nodesktop -r 'hw1("../input/scene2/", "SAM_00", "15", ".JPG", "../input/scene2/expose.txt");'
	@cd ..
	@echo "performing HDR imaging on output/scene2/aligned/"


