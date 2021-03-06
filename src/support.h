/* 
 *  Copyright (c) 2020 Zixuan Wang <zxwang@ucsd.edu>
 *  Copyright (c) 2019 Jian Yang
 *                     Regents of the University of California,
 *                     UCSD Non-Volatile Systems Lab
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SUPPORT_H
#define SUPPORT_H

#define EAX_IDX 0
#define EBX_IDX 1
#define ECX_IDX 2
#define EDX_IDX 3

#define TIME_SERIES_PATH "/tmp/"
#define POOL_SIZE	 (off_t)16 << 30
#define OPS_COUNT	 1E7
#define CORE_AFFINITY	 0

/*
 * Source: https://stackoverflow.com/questions/6121792/how-to-check-if-a-cpu-supports-the-sse3-instruction-set
 */
int avx_support()
{
	int support = 0;

	int info[4];
	__cpuid_count(0, 0, info[0], info[1], info[2], info[3]);
	int nIds = info[0];

	__cpuid_count(0x80000000, 0, info[0], info[1], info[2], info[3]);
	unsigned nExIds = info[0];

	if (nIds >= 0x00000001) {
		__cpuid_count(0x00000001, 0, info[0], info[1], info[2],
			      info[3]);
		support = (info[2] & ((int)1 << 28)) != 0 ? 1 : 0;
	}
	if (nIds >= 0x00000007) {
		__cpuid_count(0x00000007, 0, info[0], info[1], info[2],
			      info[3]);
		support = (info[1] & ((int)1 << 5)) != 0 ? 2 : support;
	}
	return support;
}

int clwb_support()
{
	int clwb_support = 0;
	unsigned cpuinfo[4] = { 0 };
	__cpuid_count(0x0, 0x0, cpuinfo[EAX_IDX], cpuinfo[EBX_IDX],
		      cpuinfo[ECX_IDX], cpuinfo[EDX_IDX]);
	if (cpuinfo[EAX_IDX] >= 0x0) {
		__cpuid_count(0x7, 0x0, cpuinfo[EAX_IDX], cpuinfo[EBX_IDX],
			      cpuinfo[ECX_IDX], cpuinfo[EDX_IDX]);
		clwb_support = (cpuinfo[EBX_IDX] & (1 << 24)) != 0;
	}
	return clwb_support;
}

#endif // SUPPORT_H
