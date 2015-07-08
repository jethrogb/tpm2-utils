/**
 * tpm2-utils -  Linux utilities for TPM 2.0 devices 
 * Copyright (C) 2015  Jethro G. Beekman
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

// ===== BEGIN TPM definitions

typedef int8_t INT8;
typedef uint8_t BOOLEAN;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

#include "Tpm20.h"

#pragma pack(1)
// From Tpm2Capability.c, same copyright as Tpm20.h
typedef struct {
	TPM2_COMMAND_HEADER       Header;
	TPM_CAP                   Capability;
	UINT32                    Property;
	UINT32                    PropertyCount;
} TPM2_GET_CAPABILITY_COMMAND;
#pragma pack()

// ===== END TPM definitions

#define ERRCHK(call) \
	if (-1==(call)) { \
		perror("Error on " #call); \
		exit(1); \
	}

static struct {
	char* str;
	UINT32 prop;
} props[]={
{"manufacturer",      TPM_PT_MANUFACTURER      },
{"vendor_string_1",   TPM_PT_VENDOR_STRING_1   },
{"vendor_string_2",   TPM_PT_VENDOR_STRING_2   },
{"vendor_string_3",   TPM_PT_VENDOR_STRING_3   },
{"vendor_string_4",   TPM_PT_VENDOR_STRING_4   },
{"vendor_tpm_type",   TPM_PT_VENDOR_TPM_TYPE   },
{"firmware_version_1",TPM_PT_FIRMWARE_VERSION_1},
{"firmware_version_2",TPM_PT_FIRMWARE_VERSION_2},
};

void usage() {
	fputs("Usage: tpm2-test [tpm-device] <property-name>\n"
	      "\n"
	      "Supported properties:\n",stderr);
	int i;
	for (i=0;i<sizeof(props)/sizeof(props[0]);i++) {
		fprintf(stderr,"\t%s\n",props[i].str);
	}
	exit(2);
}

int main(int argc,char **argv) {
	TPM2_GET_CAPABILITY_COMMAND tpm_cmd=
	{
		{
			htons(TPM_ST_NO_SESSIONS),
			htonl(sizeof(TPM2_GET_CAPABILITY_COMMAND)),
			htonl(TPM_CC_GetCapability),
		},
		htonl(TPM_CAP_TPM_PROPERTIES),
		0,
		htonl(1),
	};

	// Check command-line arguments
	if (argc<2 || argc>3) usage();

	int i;
	for (i=0;i<sizeof(props)/sizeof(props[0]);i++) {
		if (0==strcasecmp(argv[argc-1],props[i].str)) {
			tpm_cmd.Property=htonl(props[i].prop);
			break;
		}
	}
	if (i==sizeof(props)/sizeof(props[0])) usage();

	const char* tpm_device=(argc==3)?argv[1]:"/dev/tpm0";

	int fd;
	ssize_t len;
	char buf[4096];
	ERRCHK(fd=open(tpm_device,O_RDWR));
	ERRCHK(write(fd,(char*)&tpm_cmd,sizeof(tpm_cmd)));
	ERRCHK(len=read(fd,buf,4096));
	ERRCHK(write(1,buf,len));
	ERRCHK(close(fd));

	return 0;
}
