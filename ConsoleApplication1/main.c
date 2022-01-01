#include <Windows.h>
#include <stdio.h>
int main()
{
	const HFONT hFont = CreateFontA
	( /*_In_     int    cHeight        */32
	, /*_In_     int    cWidth         */0
	, /*_In_     int    cEscapement    */0
	, /*_In_     int    cOrientation   */0
	, /*_In_     int    cWeight        */0
	, /*_In_     DWORD  bItalic        */0
	, /*_In_     DWORD  bUnderline     */0
	, /*_In_     DWORD  bStrikeOut     */0
	, /*_In_     DWORD  iCharSet       */SHIFTJIS_CHARSET
	, /*_In_     DWORD  iOutPrecision  */0
	, /*_In_     DWORD  iClipPrecision */0
	, /*_In_     DWORD  iQuality       */0
	, /*_In_     DWORD  iPitchAndFamily*/0
	, /*_In_opt_ LPCSTR pszFaceName    */"ÉÅÉCÉäÉI"//"ÇlÇr ñæí©"
	);
	if (hFont) {
		const HDC hDC = CreateCompatibleDC(NULL);
		if (hDC) {
			SelectObject(hDC, hFont);
			static const char s[] = "å}èt";
			SIZE size;
			if (GetTextExtentPoint32A(hDC, s, sizeof s - 1, &size)) {
				const HBITMAP hBitmap = CreateCompatibleBitmap(hDC, size.cx, size.cy);
				if (hBitmap) {
					SelectObject(hDC, hBitmap);
					TextOutA(hDC, 0, 0, s, sizeof s - 1);
					struct {
						BITMAPINFOHEADER    bmiHeader   ;
						RGBQUAD             bmiColors[2];
					} bmi =
					{ /*BITMAPINFOHEADER    bmiHeader   ;*/
						{ /*DWORD      biSize          ;*/sizeof bmi.bmiHeader
						, /*LONG       biWidth         ;*/size.cx
						, /*LONG       biHeight        ;*/size.cy
						, /*WORD       biPlanes        ;*/1
						, /*WORD       biBitCount      ;*/1
						, /*DWORD      biCompression   ;*/BI_RGB
						, /*DWORD      biSizeImage     ;*/0
						, /*LONG       biXPelsPerMeter ;*/0
						, /*LONG       biYPelsPerMeter ;*/0
						, /*DWORD      biClrUsed       ;*/0
						, /*DWORD      biClrImportant  ;*/0
						}
					, /*RGBQUAD             bmiColors[1];*/
						{ {0x00, 0x00, 0x00} //çï
						, {0xff, 0xff, 0xff} //îí
						}
					};
					const LONG stride = (((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount + 31) & ~31) >> 3);
					const LPVOID lpvBits = malloc((size_t)stride * bmi.bmiHeader.biHeight);
					if (lpvBits) {
						const int nLines = GetDIBits
						( /*_In_                                     HDC          hdc    */hDC
						, /*_In_                                     HBITMAP      hbm    */hBitmap
						, /*_In_                                     UINT         start  */0
						, /*_In_                                     UINT         cLines */bmi.bmiHeader.biHeight
						, /*_Out_opt_                                LPVOID       lpvBits*/lpvBits
						, /*_At_((LPBITMAPINFOHEADER)lpbmi, _Inout_) LPBITMAPINFO lpbmi  */(LPBITMAPINFO )&bmi
						, /*_In_                                     UINT         usage  */DIB_RGB_COLORS
						);
						if (nLines) {
							for (long y = nLines - 1;;) {
								for (long x = 0;;) {
									// MSB     LSB
									// 1000 0000 ... 0x80
									const size_t byte_index = (stride * y) + (x >> 3);
									const BYTE   byte_data  = ((LPBYTE)lpvBits)[byte_index];
									const long   bit_index  = x & 7; //0 Å` 7
									putchar("* "[(byte_data << bit_index) & 0x80]);//ÉrÉbÉgÇ™ 0 ÇÃèÍçá
									if (++x < bmi.bmiHeader.biWidth)
										continue;
									else {
										putchar('\n');
										break;
									}
								}
								if (y == 0)
									break;
								else
									--y;
							}
						}

						free(lpvBits);
					}
				}
			}
			DeleteDC(hDC);
		}	
		DeleteObject(hFont);
	}
}
