## Link and ROM/RAM Distribution Guide

Set your Project's Startup Address at `0x90000000` just like `link.lds`

Because this Address is default address for SPI/QSPI Flash working in **Memory Map Mode** 

And do not exceed the RAM restriction (according to your Chip Datasheet)