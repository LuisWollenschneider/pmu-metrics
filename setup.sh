#!/bin/bash

# Enable rdpmc instruction at user-level
echo 2 > _rdpmc.txt
cp _rdpmc.txt /sys/devices/cpu/rdpmc
rm _rdpmc.txt

# Install required libraries
apt update
apt install cpuid msr-tools
modprobe msr

exit 0

# IMPORTANT: The following code has been moved into metrics.cpp and is executed at dynamically at runtime
# It should no longer be necessary to run this part of the setup script

# The core to program (option -c)
# By default we program core 1
CORE=1
while getopts c: flag
do
	case "${flag}" in
		c) CORE=${OPTARG};;
	esac
done

# Enable all PMU units by setting IA32_PERF_GLOBAL_CTRL register (0x38f)
if [ ${PERFEVTSEL5} ]
then
	wrmsr -p ${CORE} 0x38f 0x7000000ff
else
	wrmsr -p ${CORE} 0x38f 0x70000000f
fi

# Enable fixed counters if the flag has been set
if [[ $FIXED_CTRS_ENBL -gt 0 ]]
then
	# Enable fixed counters by setting IA32_FIXED_CTR_CTRL register (0x38d)
	wrmsr -p ${CORE} 0x38d 0x333
fi

# Program the MSRs to count specified performance events

# Set register IA32_PERFEVTSEL0 (0x186)
if [ ${PERFEVTSEL0} ]
then
	PERFEVTSEL0_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL0}))`
	wrmsr -p ${CORE} 0x186 ${PERFEVTSEL0_MASK}
	echo "wrmsr -p ${CORE} 0x186 ${PERFEVTSEL0_MASK}"
fi

# Set register IA32_PERFEVTSEL1 (0x187)
if [ ${PERFEVTSEL1} ]
then
	PERFEVTSEL1_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL1}))`
	wrmsr -p ${CORE} 0x187 ${PERFEVTSEL1_MASK}
fi

# Set register IA32_PERFEVTSEL2 (0x188)
if [ ${PERFEVTSEL2} ]
then
	PERFEVTSEL2_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL2}))`
	wrmsr -p ${CORE} 0x188 ${PERFEVTSEL2_MASK}
fi

# Set register IA32_PERFEVTSEL3 (0x189)
if [ ${PERFEVTSEL3} ]
then
	PERFEVTSEL3_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL3}))`
	wrmsr -p ${CORE} 0x189 ${PERFEVTSEL3_MASK}
fi

# Set register IA32_PERFEVTSEL4 (0x18A)
if [ ${PERFEVTSEL4} ]
then
	PERFEVTSEL4_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL4}))`
	wrmsr -p ${CORE} 0x18A ${PERFEVTSEL4_MASK}
fi

# Set register IA32_PERFEVTSEL5 (0x18B)
if [ ${PERFEVTSEL5} ]
then
	PERFEVTSEL5_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL5}))`
	wrmsr -p ${CORE} 0x18B ${PERFEVTSEL5_MASK}
	echo "wrmsr -p ${CORE} 0x18B ${PERFEVTSEL5_MASK}"
fi

# Set register IA32_PERFEVTSEL6 (0x18C)
if [ ${PERFEVTSEL6} ]
then
	PERFEVTSEL6_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL6}))`
	wrmsr -p ${CORE} 0x18C ${PERFEVTSEL6_MASK}
fi

# Set register IA32_PERFEVTSEL7 (0x18D)
if [ ${PERFEVTSEL7} ]
then
	PERFEVTSEL7_MASK=`printf "0x%X\n" $((0x430000 + ${!PERFEVTSEL7}))`
	wrmsr -p ${CORE} 0x18D ${PERFEVTSEL7_MASK}
fi
