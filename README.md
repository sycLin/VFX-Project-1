VFX Project 1
-------------

## how to run our program

1) To do image alignment, simply use the Makefile we provided.

Type in the command:
```sh
make alignAll
```
to align both sets of images in `input/scene1/` and `input/scene2`.

2) To do the HDR assembling:

```sh
cd matlab/
matlab -r "hw1('../output/scene2/aligned/', 'SAM_00', '15', '16', '.JPG', '../input/scene2/expose.txt');exit"
```

3) There will be 4 resulted files: `result.hdr`, `result.jpg`, `result.rgbe`, `mapping.fig` in the same `matlab/` directory.

