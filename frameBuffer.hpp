#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <cstdio>

class Ditter {
    private:
        int fnume;
        char *fbuff;
        int width;
        int height;
        int bytePerPixel;
        int lineLength;
        long screensize;
        // int bytes = screensize + (width * bytePerPixel);

        struct fb_var_screeninfo virtInfo;
        struct fb_fix_screeninfo fixInfo;

    public:

        Ditter() {
            fnume = open("/dev/fb0", O_RDWR);
            if (fnume == -1){
                perror("[framebuffer failed to open]");
                exit(1);
            }

            ioctl(fnume, FBIOGET_VSCREENINFO, &virtInfo);
            ioctl(fnume, FBIOGET_FSCREENINFO, &fixInfo);

            width = virtInfo.xres;
            height = virtInfo.yres;
            bytePerPixel = virtInfo.bits_per_pixel / 8;
            lineLength = fixInfo.line_length;
            screensize = height * lineLength;

            fbuff = (char*)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fnume, 0);
            if (fbuff == MAP_FAILED){
                perror("[mmap failed]");
                exit(1);
            }

        }
        ~Ditter() {
            munmap(fbuff, screensize);
            close(fnume);
        }
        void setPixel(int x, int y, int r, int g, int b) {
            if (x < 0 || x >= width || y < 0 || y >= height){
                return;
            }

            int offset = (y * lineLength) + (x * bytePerPixel);

            fbuff[offset + 0] = b;
            fbuff[offset + 1] = g;
            fbuff[offset + 2] = r;
            fbuff[offset + 3] = 255;
        }
        void clear(int r, int g, int b) {
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    setPixel(x, y, r, g, b);
                }
            }
        }
        int getWidth() { return width; }
        int getHeight() { return height; }
};
