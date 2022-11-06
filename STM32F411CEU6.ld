/* WeAct "blackpill" STM32F411CEU6 */

/* provided by CMSIS-Core */
INCLUDE gcc_arm.ld

MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

PROVIDE ( _start = main );
