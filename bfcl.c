#define _BFCL_C

#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ocl.h"
#include "ocl_brute.h"

int ocl_test();

static inline cl_ushort u16be(const unsigned char *in){
	cl_ushort out;
	unsigned char *out8 = (unsigned char *)&out;
	out8[0] = in[1];
	out8[1] = in[0];
	return out;
}

const char invalid_parameters[] = "invalid parameters\n";
const char worker_dummy_argv0[] = ">>>";

int parse_and_run(int argc, const char *argv[]) {
	int ret = 0;
	// Extremely condensed argument parsing incoming!
	if (((argc == 7 && !strcmp(argv[1], "msky")) && (!strcmp(argv[6], "sws") || !strcmp(argv[6], "rws"))) ||
			((argc == 8 && !strcmp(argv[1], "msky")) && ((!strcmp(argv[6], "sws") && !strcmp(argv[7], "sm")) || (!strcmp(argv[6], "rws") && !strcmp(argv[7], "sm"))))) {
		uint32_t msky[4], ver[4], msky_offset, msky_max_offset;
		hex2bytes((unsigned char*)msky, 16, argv[2], 1);
		hex2bytes((unsigned char*)ver, 16, argv[3], 1);
		hex2bytes((unsigned char*)&msky_offset, 4, argv[4], 1);
		hex2bytes((unsigned char*)&msky_max_offset, 4, argv[5], 1);
		if ((argc == 7 || argc == 8) && !strcmp(argv[6], "rws")) {
			reduced_work_size_mode = 1;
		}
		if (argc == 8 && !strcmp(argv[7], "sm")) {
			seedminer_mode = 1;
		}
		ret = ocl_brute_msky(msky, ver, msky_offset, msky_max_offset);
	// More extremely condensed argument parsing incoming!
	} else if (((argc == 7 && !strcmp(argv[1], "lfcs")) && (!strcmp(argv[6], "sws") || !strcmp(argv[6], "rws"))) ||
			((argc == 8 && !strcmp(argv[1], "lfcs")) && ((!strcmp(argv[6], "sws") && !strcmp(argv[7], "sm")) || (!strcmp(argv[6], "rws") && !strcmp(argv[7], "sm")))) ||
			((argc == 9 && !strcmp(argv[1], "lfcs")) && (!strcmp(argv[8], "sws") || !strcmp(argv[8], "rws"))) ||
			((argc == 10 && !strcmp(argv[1], "lfcs")) && ((!strcmp(argv[8], "sws") && !strcmp(argv[9], "sm")) || (!strcmp(argv[8], "rws") && !strcmp(argv[9], "sm"))))) {
		uint32_t lfcs, ver[2], lfcs_offset;
		uint16_t newflag;
		uint32_t upper_bound = 0, lower_bound = 0;
		hex2bytes((unsigned char*)&lfcs, 4, argv[2], 1);
		hex2bytes((unsigned char*)&newflag, 2, argv[3], 1);
		hex2bytes((unsigned char*)ver, 8, argv[4], 1);
		hex2bytes((unsigned char*)&lfcs_offset, 4, argv[5], 1);
		if (argc == 9 || argc == 10) {
			hex2bytes((unsigned char*)&upper_bound, 4, argv[6], 1);
			hex2bytes((unsigned char*)&lower_bound, 4, argv[7], 1);
		}
		if (((argc == 7 || argc == 8) && !strcmp(argv[6], "rws")) ||
				((argc == 9 || argc == 10) && !strcmp(argv[8], "rws"))) {
			reduced_work_size_mode = 1;
		}
		if ((argc == 8 && !strcmp(argv[7], "sm")) ||
				(argc == 10 && !strcmp(argv[9], "sm"))) {
			seedminer_mode = 1;
		}
		ret = ocl_brute_lfcs(lfcs, newflag, ver, lfcs_offset, upper_bound, lower_bound);
	} else if (argc == 7) {
		unsigned char console_id[8], emmc_cid[16], offset[2], src[16], ver[16];
		hex2bytes(console_id, 8, argv[2], 1);
		hex2bytes(emmc_cid, 16, argv[3], 1);
		hex2bytes(offset, 2, argv[4], 1);
		hex2bytes(src, 16, argv[5], 1);
		hex2bytes(ver, 16, argv[6], 1);

		if (!strcmp(argv[1], "console_id")) {
			ret = ocl_brute_console_id(console_id, emmc_cid, u16be(offset), src, ver, 0, 0, 0, NORMAL);
		} else if (!strcmp(argv[1], "console_id_bcd")) {
			ret = ocl_brute_console_id(console_id, emmc_cid, u16be(offset), src, ver, 0, 0, 0, BCD);
		} else if (!strcmp(argv[1], "console_id_3ds")) {
			ret = ocl_brute_console_id(console_id, emmc_cid, u16be(offset), src, ver, 0, 0, 0, CTR);
		} else if (!strcmp(argv[1], "emmc_cid")) {
			ret = ocl_brute_emmc_cid(console_id, emmc_cid, u16be(offset), src, ver);
		} else {
			puts(invalid_parameters);
			ret = -1;
		}
	} else if (argc == 9) {
		unsigned char console_id[8],
			offset0[2], src0[16], ver0[16],
			offset1[2], src1[16], ver1[16];
		hex2bytes(console_id, 8, argv[2], 1);
		hex2bytes(offset0, 2, argv[3], 1);
		hex2bytes(src0, 16, argv[4], 1);
		hex2bytes(ver0, 16, argv[5], 1);
		hex2bytes(offset1, 2, argv[6], 1);
		hex2bytes(src1, 16, argv[7], 1);
		hex2bytes(ver1, 16, argv[8], 1);
		
		if (!strcmp(argv[1], "console_id")) {
			ret = ocl_brute_console_id(console_id, 0,
				u16be(offset0), src0, ver0, u16be(offset1), src1, ver1, NORMAL);
		} else if (!strcmp(argv[1], "console_id_bcd")) {
			ret = ocl_brute_console_id(console_id, 0,
				u16be(offset0), src0, ver0, u16be(offset1), src1, ver1, BCD);
		} else if (!strcmp(argv[1], "console_id_3ds")) {
			ret = ocl_brute_console_id(console_id, 0,
				u16be(offset0), src0, ver0, u16be(offset1), src1, ver1, CTR);
		} else {
			puts(invalid_parameters);
			ret = -1;
		}
	} else {
		printf(invalid_parameters);
		ret = -1;
	}
	return ret;
}

void worker_loop() {
	char buf[256];
	int argc = 1, ret;
	char *argv[16], c, *p, *arg;
	argv[0] = (char *)worker_dummy_argv0;
	printf("worker mode enabled\n");
	fflush(stdout);
	while (1) {
		printf(">>>\n");
		fflush(stdout);
		fgets(buf, 256, stdin);
		if (strlen(buf) == 255) while ((c = fgetc(stdin)) != EOF && c != '\n');
		argc = 1;
		for (p=buf, arg=p; p<buf+256 && argc<=16; p++) {
			if (*p == ' ' || *p == 0 || *p == '\r' || *p == '\n') {
				argv[argc++] = arg;
				if (*p == 0) {
					break;
				} else if (*p == '\r' || *p == '\n') {
					*p = 0;
					break;
				} else {
					*p = 0;
				}
				for (arg=++p; *p == ' '; arg=++p);
			}
		}
		if (argc == 2 && !strcmp(argv[1], "exit")) {
			printf("Exit.\n");
			break;
		} else {
			ret = parse_and_run(argc, (const char **)argv);
			printf("||| %d\n", ret);
			fflush(stdout);
		}
	}
}

int main(int argc, const char *argv[]) {
	stop_bfcl = 0; // Not really used at the moment
	seedminer_mode = 0;
	reduced_work_size_mode = 0;
	stdio_mode = 0;
	int ret = 0;
	if (argc == 1) {
		ret = ocl_test();
	} else if (argc == 2 && !strcmp(argv[1], "info")) {
		cl_uint num_platforms;
		ocl_info(&num_platforms, 1);
	} else if (((argc == 3 && !strcmp(argv[1], "worker")) && (!strcmp(argv[2], "sws") || !strcmp(argv[2], "rws"))) ||
			((argc == 4 && !strcmp(argv[1], "worker")) && (!strcmp(argv[2], "sws") || !strcmp(argv[2], "rws")) && (!strcmp(argv[3], "stdio")))) {
		worker_mode = 1;
		if ((argc == 3 || argc == 4) && !strcmp(argv[2], "rws")) {
			reduced_work_size_mode = 1;
		}
		if (argc == 4 && !strcmp(argv[3], "stdio")) {
			stdio_mode = 1;
		}
		worker_loop();
		ocl_brute_free_worker();
		ret = 0;
	} else {
		ret = parse_and_run(argc, argv);
	}
	return ret;
}
