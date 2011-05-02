/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                       Datatypes
*/

/**
 * Makes a Color
 * @param col the color in form "0xfb"
 * @return the Color for method cheColor in io.c
 */
Color toColor(unsigned short col)
{
	Color ret;
	ret.fg = col & 0xF;
	ret.bg = (col & 0xF0) >> 4;
	return ret;
}

/**
 * Creates a Dimension
 * @param x width
 * @param y height
 * @return Dimension
 */
Dimension newDimension(int x, int y)
{
	Dimension dim;
	dim.width = x;
	dim.height = y;
	return dim;
}
