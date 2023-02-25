# crossPHL
C programs that run on the Portable Hearing Laboratory (PHL). Centered on ALSA diagnostics.

Using arm-linux-gnueabihf-gcc

Linking libraries on PHL by network connection to PHL (~/openMHA/mha/tools/connect....internet.sh) and then:

sshfs mha@192.168.7.2:/ /home/nick/crossPHL/PHL -o transform_symlinks

Compile .c program with debug symbols, link library from PHL, link asound and math libraries:
\~/crossPHL$ arm-linux-gnueabihf-gcc -g -o pcm_min pcm_min.c -L~/crossPHL/PHL/usr/lib/arm-linux-gnueabihf -lasound -lm

