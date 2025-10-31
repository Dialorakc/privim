#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <cstdio>

class Ditter {
    private:
        int fnume = open("/dev/fb0", O_RDWR);
    public:

        int buffer() {
            struct fb_var_screeninfo virtInfo;
            struct fb_fix_screeninfo fixInfo;

            if (fnume == -1){
                perror("[framebuffer failed to open]");
                exit(1);
            }

            ioctl(fnume, FBIOGET_VSCREENINFO, &virtInfo);
            ioctl(fnume, FBIOGET_FSCREENINFO, &fixInfo);

            int width = virtInfo.xres;
            int height = virtInfo.yres;
            int bytePerPixel = virtInfo.bits_per_pixel / 8;
            int lineLength = fixInfo.line_length;
            int screensize = height * lineLength;
            int bytes = screensize + (width * bytePerPixel);

            char *fbuff = (char*)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fnume, 0);

            if (fbuff == MAP_FAILED){
                perror("[mmap failed]");
                exit(1);
            }

            return 0;
        }
};
