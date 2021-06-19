


# PIANO FILE BINARY FORMAT  
  
Bit = 0-1
Nibble = 0-15 (4 bits)
Byte = 0-255 (8 bits)
Short = 0-65535 (16 bits)
Int = 0-4294967295 (32 bits)


* FILE
    * file type (1 byte) (Always make '2' for binary format)
    * header
        * header validation (3 bytes) (always make 0x79BDAC)
        * song name length (1 byte)
        * song name (same amount of bytes as song name length)
        * song description length (2 bytes)
        * song description (same amount of bytes as song description length)
    * song array
        * ACTIONS (1 nibble (half byte) for action type)
            * 0x1 (play keys)
                * next nibble is amount of keys
                * the bytes after that is the keys
            * 0x2 (delay)
                * next nibble: 0x1 if byte, 0x2 if short, 0x3 if int
                * the byte/short/int after that is delay
            * 0xA-0xF (meta action 1 byte)
                * 0xFF (End song)