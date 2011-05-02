/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                    CMOS
*/

 
/**
 * Liest ein Byte aus den CMOS
 *  @param offset Offset im CMOS
 *  @return Gelesener Wert
 */
unsigned char cmos_read(unsigned char offset) {
	uint8_t tmp = inb(CMOS_PORT_ADDRESS);
	outb(CMOS_PORT_ADDRESS, (tmp & 0x80) | (offset & 0x7F));
	return inb(CMOS_PORT_DATA);
}
 
/**
 * Schreibt ein Byte in das CMOS
 *  @param offset Offset im CMOS
 *  @param val Zu schreibender Wert
 */
void cmos_write(unsigned char offset, unsigned char val) {
	unsigned char tmp = inb(CMOS_PORT_ADDRESS);
	outb(CMOS_PORT_ADDRESS, (tmp & 0x80) | (offset & 0x7F));
	outb(CMOS_PORT_DATA,val);
}
 
