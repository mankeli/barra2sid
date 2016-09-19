#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int year = 0;
	if (argc < 4)
	{
		printf("barra2sid (C) 2016 mankeli/extend.\nusage: %s biisi.prg biisi.sid year\n", argv[0]);
		return 1;
	}

	FILE *fi = fopen(argv[1], "rb");
	FILE *fo = fopen(argv[2], "wb");
	year = atoi(argv[3]);

	uint16_t loadaddr;
	fread(&loadaddr, 1, 2, fi);

	char header[1024];

	header[0] = 0x50;
	header[1] = 0x53;
	header[2] = 0x49;
	header[3] = 0x44;

	header[4] = 0;
	header[5] = 1;

	header[6] = 0;
	header[7] = 0x76;

	header[8] = (loadaddr >> 8) & 0xFF;
	header[9] = loadaddr & 0xFF;

	header[0xA] = 0x10;
	header[0xB] = 0x00;
	header[0xC] = 0x10;
	header[0xD] = 0x03;

	header[0xE] = 0x00;
	header[0xF] = 0x01;
	header[0x10] = 0x00;
	header[0x11] = 0x01;

	header[0x12] = 0xFF;
	header[0x13] = 0xFF;
	header[0x14] = 0xFF;
	header[0x15] = 0xFF;

	strcpy(header + 0x16, argv[1]);
	strcpy(header + 0x36, "Toni Nisula (Barracuda)");
	snprintf(header + 0x56, 0x1F, "%i Extend", year);

	fwrite(header, 1, 0x76, fo);

	//fprintf(fo, "moi :D\n");

	char buf[65536];

	int read;

	read = fread(buf, 1, 65536, fi);
	printf("song data size: %04X\n", read);

	int initoffs = 0x1000 - loadaddr;
/*
.C:1000  4C 6D 16    JMP $166D
.C:1003  4C BD 10    JMP $10BD
.C:1006  A9 64       LDA #$64
.C:1008  8D 04 DC    STA $DC04
.C:100b  A9 26       LDA #$26
.C:100d  8D 05 DC    STA $DC05
.C:1010  4C AD DE    JMP $DEAD
*/

	buf[initoffs+6] = 0xA9;
	buf[initoffs+7] = 0x64;
	buf[initoffs+8] = 0x8D;
	buf[initoffs+9] = 0x04;
	buf[initoffs+10] = 0xDC;

	buf[initoffs+11] = 0xA9;
	buf[initoffs+12] = 0x26;
	buf[initoffs+13] = 0x8D;
	buf[initoffs+14] = 0x05;
	buf[initoffs+15] = 0xDC;

	buf[initoffs+16] = buf[initoffs+0];
	buf[initoffs+17] = buf[initoffs+1];
	buf[initoffs+18] = buf[initoffs+2];

	buf[initoffs+1] = 0x06;
	buf[initoffs+2] = 0x10;


	fwrite(buf, 1, read, fo);

	fclose(fi);
	fclose(fo);
	return 0;
}