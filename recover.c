#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    
    const int buffer_size = 512;
    unsigned char  buffer[buffer_size];
    
    int file_index = 0;
    
    const int output_file_size = 8;
    char filename[output_file_size];
    //fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    FILE *outptr = NULL;
    
    while(fread(buffer, buffer_size, 1, inptr) == 1)
    {
        if(buffer[0] == 0xFF &&
            buffer[1] == 0xD8 &&
            buffer[2] == 0xFF &&
            (buffer[3] & 0xF0) == 0xE0)
            {
                if(outptr != 0)
                {
                    fclose(outptr);
                }
                sprintf(filename, "%03d.jpg", file_index);
                //printf("%s\n", filename);
                file_index++;
                
                outptr = fopen(filename, "w");
            }
            
            if(outptr != NULL)
            {
                fwrite(buffer, buffer_size, 1, outptr);
            }
    }
    
    fclose(outptr);
    fclose(inptr);
    return 0;
}