#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/system.h>
#include <ogc/lwp_watchdog.h>


static void *xfb = NULL;
static GXRModeObj *Vmode = NULL;

void sleep(float delay) {
	float ticks = delay/20;
	float start = 0;

	while (start < ticks) {
		start++;
		VIDEO_WaitVSync();
	}
}
static void VideoStart() {

	VIDEO_Init();

	WPAD_Init();

	Vmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(Vmode));

	console_init(xfb, 20, 20, Vmode->fbWidth, Vmode->xfbHeight, Vmode->fbWidth * VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(Vmode);

	VIDEO_SetNextFramebuffer(xfb);

	VIDEO_SetBlack(false);

	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(Vmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
}

static void PrintHletter() {

	printf("\x1b[5;20H");

	for (size_t X = 20; X < 37; X++)	{

		if (X < 23) {
			for (size_t Y = 5; Y < 20; Y++) {
				printf("\x1b[%d;%dH", Y, X);
				printf("h");
				sleep(10);
			}

		} else if (X < 26 || (X > 32 && X < 37) ) {

			if (X <= 35) {
				for (size_t Y = 11; Y < 13; Y++) {
					printf("\x1b[%d;%dH", Y, X);
					printf("h");
					sleep(10);
				}
			}

			if (X >= 34) {
				for (size_t Y = 13; Y < 20; Y++) {
					printf("\x1b[%d;%dH", Y, X);
					printf("h");
					sleep(10);
				}
			}

		} else if (X <= 32 && X >= 26) {

			for (size_t Y = 9; Y < 11; Y++) {
				printf("\x1b[%d;%dH", Y, X);
				printf("h");
				sleep(10);
			}

		}		
		
	}
	
}

int main(int argc, char **argv) {

	VideoStart();

	
	PrintHletter();	

	while(1) {

		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_HOME ) {
			exit(0);
		}
		VIDEO_WaitVSync();
	}

	return 0;
}