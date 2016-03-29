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
	# for un-aligned images
	@echo "performing HDR imaging on input/scene1/"
	@cd matlab
	@matlab -r "hw1('../input/scene1/', '', '1', '8', '.JPG', '../input/scene1/detail.txt');exit"
	@echo "moving results to output/scene1/"
	@mv mapping.fig result.jpg result.hdr result.rgbe ../output/scene1/
	@cd ..
	# for aligned images
	@echo "performing HDR imaging on output/scene1/aligned/"
	@cd matlab
	@matlab -r "hw1('../output/scene1/aligned/', '', '1', '8', '.JPG', '../input/scene1/detail.txt');exit"
	@echo "moving results to output/scene1/aligned/"
	@mv mapping.fig result.jpg result.hdr result.rgbe ../output/scene1/aligned/
	@cd ..
	
HDR2:
	@echo "performing HDR imaging on input/scene2/"
	@cd matlab
	@matlab -r "hw1('../input/scene2/', 'SAM_00', '15', '16', '.JPG', '../input/scene2/expose.txt');exit"
	@echo "moving results to output/scene2/"
	@mv mapping.fig result.jpg result.hdr result.rgbe ../output/scene2/
	@cd ..
	@echo "performing HDR imaging on output/scene2/aligned/"
	@cd matlab
	@matlab -r "hw1('../output/scene2/aligned/', 'SAM_00', '15', '16', '.JPG', '../input/scene2/expose.txt');exit"
	@echo "moving results to output/scene2/aligned/"
	@mv mapping.fig result.jpg result.hdr result.rgbe ../output/scene2/aligned/
	@cd ..


