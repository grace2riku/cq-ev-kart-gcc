	/* defines monitorromsym symbol */
	.section .MonitorROM, "a"
	.space 0x800, 0xff

	/* Secures 16 byte space for monitor RAM section */
	.section .MonitorRAM, "aw"
	.lcomm monitorramsym, 16
