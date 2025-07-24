import sys

def parse_bdf(input_file, output_file):
    with open(input_file, "r") as bdf, open(output_file, "wb") as binary:
        in_bitmap_section = False
        for line in bdf:
            line = line.strip()
            
            # Start of a glyph definition
            if line.startswith("STARTCHAR"):
                print(f"Processing glyph: {line.split(' ')[1]}")
            
            # Entering bitmap section
            elif line == "BITMAP":
                in_bitmap_section = True
            
            # Exiting glyph definition
            elif line == "ENDCHAR":
                in_bitmap_section = False
            
            # Process bitmap data
            elif in_bitmap_section:
                # Convert hex string to binary and write to file
                binary.write(bytes.fromhex(line))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python bdf_to_binary.py <input.bdf> <output.bin>")
        sys.exit(1)

    input_bdf = sys.argv[1]
    output_bin = sys.argv[2]
    parse_bdf(input_bdf, output_bin)
    print(f"Binary font written to {output_bin}")
