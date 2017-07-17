/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize scale infile outfile\n");
        return 1;
    }
    
    int scale_factor = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    (void)scale_factor;
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    int old_height = bi.biHeight;
    int old_width = bi.biWidth;
    int old_padding = (4 - (old_width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int new_height = old_height * scale_factor;
    int new_width = old_width * scale_factor;
    int new_padding = (4 - (new_width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int new_file_size = sizeof(BITMAPFILEHEADER) +
                        sizeof(BITMAPINFOHEADER) +
                        new_width * abs(new_height) * sizeof(RGBTRIPLE) +
                        new_padding * abs(new_height);
                        
    //(void)new_file_size;
    bf.bfSize = new_file_size;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    bi.biWidth = new_width;
    bi.biHeight = new_height;
    bi.biSizeImage  = new_width * abs(new_height) * sizeof(RGBTRIPLE) + new_padding * abs(new_height);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    //int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    RGBTRIPLE* row = malloc(new_width * sizeof(RGBTRIPLE));

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_height); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < old_width; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            for(int k = 0; k < scale_factor; k++)
            {
                row[j * scale_factor + k] = triple;
            }
            
            
            // temporary storage
            //RGBTRIPLE triple;

            // read RGB triple from infile
            //fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            //fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        
        for(int k = 0; k < scale_factor; k++)
            {
                fwrite(row, sizeof(RGBTRIPLE) * new_width, 1, outptr);
                for (int pad_index  = 0; pad_index < new_padding; pad_index++)
                {
                    fputc(0x00, outptr);
                }
            }

        // skip over padding, if any
        fseek(inptr, abs(old_padding), SEEK_CUR);
        
        // then add it back (to demonstrate how)
        //for (int k = 0; k < padding; k++)
        //{
        //    fputc(0x00, outptr);
        //}
    }
    
    //by IKR
    free(row);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}