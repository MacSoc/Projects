#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;

const int  sobel_0[3][3] = {
	{ -1, 0, 1},
	{ -2, 0, 2},
	{ -1, 0, 1} };

const int sobel_1[3][3] = {
{ 0, 1, 2},
{ -1, 0, 1},
{ -2, -1, 0} };

const int sobel_2[3][3] = {
{ 1, 2, 1},
{ 0, 0, 0},
{ -1, -2, -1} };

const int sobel_3[3][3] = {
{ 2, 1, 0},
{ 1, 0, -1},
{ 0, -1, -2} };

const int sobel_4[3][3] = {
{1, 0, -1},
{ 2, 0, -2},
{ 1, 0, -1} };

const int sobel_5[3][3] = {
{0, -1, -2},
{ 1, 0, -1},
{ 2, 1, 0} };

const int sobel_6[3][3] = {
{-1, -2, -1},
{0, 0, 0},
{1, 2, 1} };

const int sobel_7[3][3] = {
{ -2, -1, 0},
{-1, 0, 1},
{0, 1, 2} };

struct BMPFileHeader {
	uint16_t bfType;          // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
	uint32_t bfSize;               // Size of the file (in bytes)
	uint16_t bfReserved1;               // Reserved, always 0
	uint16_t bfReserved2;               // Reserved, always 0
	uint32_t bfOffBits;             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
	uint32_t size;                      // Size of this header (in bytes)
	uint32_t width;                      // width of bitmap in pixels
	uint32_t height;                     // width of bitmap in pixels
											 //       (if positive, bottom-up, with origin in lower left corner)
											 //       (if negative, top-down, with origin in upper left corner)
	uint16_t planes;                    // No. of planes for the target device, this is always 1
	uint16_t bit_count;                 // No. of bits per pixel
	uint32_t compression;               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	uint32_t size_image;                // 0 - for uncompressed images
	uint32_t x_pixels_per_meter;
	uint32_t y_pixels_per_meter;
	uint32_t colors_used;               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	uint32_t colors_important;          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

void odczytajBFH(ifstream& ifs, char *nazwa_in, BMPFileHeader& bfh)
{
	ifs.open(nazwa_in, ios::binary);
	if (ifs.good() == false)
	{
		cout << "Zle otwarcie pliku graficznego BMP !" << endl;

	}

	cout << '\n';
	ifs.read(reinterpret_cast<char*>(&bfh.bfType), 2);						// obsada wzskaznika na strukture bf.type w roli zmiennej typu char
	ifs.read(reinterpret_cast<char*>(&bfh.bfSize), 4);
	ifs.read(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	ifs.read(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	ifs.read(reinterpret_cast<char*>(&bfh.bfOffBits), 4);

	cout << "bf_Type :  [" << bfh.bfType << "]  (Sygnatura pliku '19778' dla prawidlowego pliku BMP, przechowywany heksadecymalnie)" << endl;
	cout << "bf_Size :  [" << bfh.bfSize << "]  (Dlugosc calego pliku w bajtach)" << endl;
	cout << "bf_Reserved1 :  [" << bfh.bfReserved1 << "]  (Pole zarezerwowane, zwykle 0)" << endl;
	cout << "bf_Reserved2 :  [" << bfh.bfReserved2 << "]  (Pole zarezerwowane, zwykle 0)" << endl;
	cout << "bf_OffBits :  [" << bfh.bfOffBits << "]  (Pozycja danych obrazowych pliku)" << endl;
}

void odczytajBIH(ifstream& ifs, BMPInfoHeader& bih)
{

	ifs.read(reinterpret_cast<char*>(&bih.size), 4);					
	ifs.read(reinterpret_cast<char*>(&bih.width), 4);
	ifs.read(reinterpret_cast<char*>(&bih.height), 4);
	ifs.read(reinterpret_cast<char*>(&bih.planes), 2);
	ifs.read(reinterpret_cast<char*>(&bih.bit_count), 2);
	ifs.read(reinterpret_cast<char*>(&bih.compression), 4);
	ifs.read(reinterpret_cast<char*>(&bih.size_image), 4);
	ifs.read(reinterpret_cast<char*>(&bih.x_pixels_per_meter), 4);
	ifs.read(reinterpret_cast<char*>(&bih.y_pixels_per_meter), 4);
	ifs.read(reinterpret_cast<char*>(&bih.colors_used), 4);
	ifs.read(reinterpret_cast<char*>(&bih.colors_important), 4);

	cout << endl;
	cout << "bi_Size :  [" << bih.size << "]  (Rozmiar naglowka informacyjnego)" << endl;
	cout << "bi_Width :  [" << bih.width << "]  (Szerokosc obrazu w pikselach)" << endl;
	cout << "bi_Height :  [" << bih.height << "]  (Wysokosc obrazu w pikselach)" << endl;
	cout << "bi_Planes :  [" << bih.planes << "]  (Liczba platow, musi byc 1)" << endl;
	cout << "bi_Bit_count :  [" << bih.bit_count << "]  (Liczba bitow na piksel: 1, 4, 8, 16, 25, lub 32)" << endl;
	cout << "bi_Compression :  [" << bih.compression << "]  (Algorytm kompresji: 0 lub 3 - nieskompresowany plik)" << endl;
	cout << "bi_Size_image :  [" << bih.size_image << "]  (Rozmiar rysunku)" << endl;
	cout << "bi_X_pixels_per_meter :  [" << bih.x_pixels_per_meter << "]  (Rozdzielczosc pozioma)" << endl;
	cout << "bi_Y_pixels_per_meter :  [" << bih.y_pixels_per_meter << "]  (Rozdzielczosc pionowa)" << endl;
	cout << "bi_Colors_used :  [" << bih.colors_used << "]  (Liczba kolorow w palecie)" << endl;
	cout << "bi_Colors_important :  [" << bih.colors_important << "]  (Liczba waznych kolorow w palecie)" << endl;
}

void Sobel_R(char*& R_Nowe, char*& R, const int szerokosc, const int wysokosc)
{
	int min = 0;
	int max = 255;
	unsigned int suma = 0;
	for (int i = 1; i < wysokosc - 1; i++)
	{
		for (int j = 1; j < szerokosc - 1; j++)
		{
			// 0 direction 
			suma = 0;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_0[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 45 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_1[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 90 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_2[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 135 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_3[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 180 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_4[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 225 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_5[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 270 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_6[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}

			// 315 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + R[szerokosc * (i + x) + j + y] * sobel_7[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > R[szerokosc * i + j])
			{
				R_Nowe[szerokosc * i + j] = suma;
			}
		}
	}
}

void Sobel_G(char*& G_Nowe, char*& G, const int szerokosc, const int wysokosc)
{
	int min = 0;
	int max = 255;
	unsigned int suma = 0;
	for (int i = 1; i < wysokosc - 1; i++)
	{
		for (int j = 1; j < szerokosc - 1; j++)
		{
			// 0 direction
			suma = 0;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_0[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 45 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_1[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 90 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_2[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 135 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_3[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 180 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_4[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 225 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_5[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 270 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_6[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}

			// 315 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + G[szerokosc * (i + x) + j + y] * sobel_7[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > G[szerokosc * i + j])
			{
				G_Nowe[szerokosc * i + j] = suma;
			}
		}
	}
}

void Sobel_B(char*& B_Nowe, char*& B, const int szerokosc, const int wysokosc)
{
	int min = 0;
	int max = 255;
	unsigned int suma = 0;
	for (int i = 1; i < wysokosc - 1; i++)
	{
		for (int j = 1; j < szerokosc - 1; j++)
		{
			// 0 direction
			suma = 0;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_0[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}
		
			// 45 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_1[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 90 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_2[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 135 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_3[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 180 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_4[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 225 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_5[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 270 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_6[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}

			// 315 direction
			suma = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					suma = suma + B[szerokosc * (i + x) + j + y] * sobel_7[x + 1][y + 1];
				}
			}
			if (suma > max)
			{
				suma = max;
			}
			if (suma < min)
			{
				suma = min;
			}
			if (suma > B[szerokosc * i + j])
			{
				B_Nowe[szerokosc * i + j] = suma;
			}
		}
	}
}

void zapiszBFH(ofstream& ofs, char* nazwa_out, BMPFileHeader& bfh)
{
	ofs.open(nazwa_out, ios::binary);
	if (ofs.good() == false)
	{
		cout << "Zle otwarcie pliku graficznego BMP !" << endl;

	}

	ofs.write(reinterpret_cast<char*>(&bfh.bfType), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfSize), 4);
	ofs.write(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfOffBits), 4);
}

void zapiszBIH(ofstream& ofs, BMPInfoHeader& bih)
{

	ofs.write(reinterpret_cast<char*>(&bih.size), 4);
	ofs.write(reinterpret_cast<char*>(&bih.width), 4);
	ofs.write(reinterpret_cast<char*>(&bih.height), 4);
	ofs.write(reinterpret_cast<char*>(&bih.planes), 2);
	ofs.write(reinterpret_cast<char*>(&bih.bit_count), 2);
	ofs.write(reinterpret_cast<char*>(&bih.compression), 4);
	ofs.write(reinterpret_cast<char*>(&bih.size_image), 4);
	ofs.write(reinterpret_cast<char*>(&bih.x_pixels_per_meter), 4);
	ofs.write(reinterpret_cast<char*>(&bih.y_pixels_per_meter), 4);
	ofs.write(reinterpret_cast<char*>(&bih.colors_used), 4);
	ofs.write(reinterpret_cast<char*>(&bih.colors_important), 4);
}

int main()
{
	char nazwa_in[80];
	char nazwa_out[80];
	BMPFileHeader bfh;
	BMPInfoHeader bih;
	ifstream ifs;
	ofstream ofs;
	
	cout << "Podaj nazwe pliku wejsciowego: ";
	cin >> nazwa_in;							//	obrazek.bmp			
	cout << "Podaj nazwe pliku wyjsciowego: ";
	cin >> nazwa_out;								// obrazekcopy.bmp

	odczytajBFH(ifs, nazwa_in, bfh);
	odczytajBIH(ifs, bih);

	char* R = new char[bih.width * bih.height];			// tablica alokowana dynamicznie na wskazniki typu char
	char* G = new char[bih.width * bih.height];
	char* B = new char[bih.width * bih.height];
	char* C = new char[bih.width * bih.height];			// tablica alokowana dynamicznie na wskazniki typu char na dodatkowe zera

	char* R_Nowe = new char[bih.width * bih.height];
	char* G_Nowe = new char[bih.width * bih.height];
	char* B_Nowe = new char[bih.width * bih.height];

	const int wysokosc = bih.height;	
	const int szerokosc = bih.width;	
	int zera_na_wiersz = 0;

	if(szerokosc % 4 != 0)		
	{
		int bajty_na_wiersz = szerokosc * 3;
		zera_na_wiersz = (4 - (bajty_na_wiersz % 4));
	}

	int a = 0;
	for (int j = 0; j < bih.height; j++)					// odczytywanie zmiennych rgb
	{
		for (int i = 0; i < bih.width; i++)
		{
			ifs.read(reinterpret_cast<char*>(&B[a]), 1);		// kazda skladowa koloru po 1 bajcie
			ifs.read(reinterpret_cast<char*>(&G[a]), 1);
			ifs.read(reinterpret_cast<char*>(&R[a]), 1);
			a++;
		}
		for (int i = 0; i < zera_na_wiersz; i++)
		{
			ifs.read(reinterpret_cast<char*> (&C[i]), 1);
		}
	}

	for (int i = 0; i < szerokosc * wysokosc; i += 1)
	{
		R_Nowe[i] = R[i];
		G_Nowe[i] = G[i];
		B_Nowe[i] = B[i];
	}

	Sobel_B(B_Nowe, B, szerokosc, wysokosc);
	Sobel_G(G_Nowe, G, szerokosc, wysokosc);
	Sobel_R(R_Nowe, R, szerokosc, wysokosc);
	zapiszBFH(ofs, nazwa_out, bfh);
	zapiszBIH(ofs, bih);
	
	int b = 0;
	for (int j = 0; j < bih.height; j++)				// zapis nowych danych rgb
	{
		for (int i = 0; i < bih.width; i++)
		{
			ofs.write(reinterpret_cast<char*>(&B_Nowe[b]), 1);
			ofs.write(reinterpret_cast<char*>(&G_Nowe[b]), 1);
			ofs.write(reinterpret_cast<char*>(&R_Nowe[b]), 1);
			b++;
		}
		for (int i = 0; i < zera_na_wiersz; i++)
		{
			ofs.write(reinterpret_cast<char*> (&C[i]), 1);
		}
	}

	delete[] B;
	delete[] G;
	delete[] R;
	delete[] B_Nowe;
	delete[] G_Nowe;
	delete[] R_Nowe;

	ifs.close();
	ofs.close();

	return 0;
}