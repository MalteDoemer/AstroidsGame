#pragma once
#include "olcVec2d.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <vector>
#define MAX_KEYS 128

typedef v2d_generic<short> vs2d;
typedef CHAR_INFO Pixel;

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "kernel32.lib")

#pragma region Util

template <char seperator = '\0', char ending = '\n'>
void printd()
{
	std::stringstream s;
	s << ending;
	OutputDebugStringA(s.str().c_str());
}

template <char seperator = '\0', char ending = '\n', typename T, typename... Types>
void printd(T var1, Types... var2)
{
	std::stringstream s;
	s << var1 << seperator;
	OutputDebugStringA(s.str().c_str());

	printd<seperator, ending>(var2...);
}

#pragma endregion

const double HALF_PI = 1.5707963267948966;
const double PI = 3.141592653589793;

enum COLOR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXELS
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

namespace Pixels
{
	Pixel FromGray(int8_t grey)
	{
		wchar_t sym = L' ';
		short bg_col = 0;
		short fg_col = 0;

		float luminance = (float)grey / 255.0f;
		int pixel_bw = (int)(luminance * 13.0f);
		switch (pixel_bw)
		{
		case 0:
			bg_col = BG_BLACK;
			fg_col = FG_BLACK;
			sym = PIXEL_SOLID;
			break;

		case 1:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_QUARTER;
			break;
		case 2:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_HALF;
			break;
		case 3:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 4:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_SOLID;
			break;

		case 5:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_QUARTER;
			break;
		case 6:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_HALF;
			break;
		case 7:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 8:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_SOLID;
			break;

		case 9:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_QUARTER;
			break;
		case 10:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_HALF;
			break;
		case 11:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 12:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_SOLID;
			break;
		}

		return {sym, (WORD)(fg_col | bg_col)};
	}
	// Pixel FromRGB(int8_t R, int8_t G, int8_t B)
	// {
	//
	// 	float r = (float)R / 255.0f;
	// 	float b = (float)G / 255.0f; // Had to do a slight change here.
	// 	float g = (float)B / 255.0f;
	//
	// 	wchar_t sym;
	//
	// 	short fg_col;
	// 	short bg_col;
	//
	// 	// Is pixel coloured (i.e. RGB values exhibit significant variance)
	// 	float fMean = (r + g + b) / 3.0f;
	// 	float fRVar = (r - fMean)*(r - fMean);
	// 	float fGVar = (g - fMean)*(g - fMean);
	// 	float fBVar = (b - fMean)*(b - fMean);
	// 	float fVariance = fRVar + fGVar + fBVar;
	//
	// 	if (fVariance < 0.0001f)
	// 	{
	// 		return FromGray((int8_t)(fMean * 255));
	// 	}
	// 	else
	// 	{
	// 		// Pixel has colour so get dominant colour
	// 		float y = min(r, g);// (r * g);
	// 		float c = min(g, b);// (g * b);
	// 		float m = min(b, r);// (b * r);
	//
	// 		float fMean2 = (y + c + m) / 3.0f;
	// 		float fYVar = (y - fMean2)*(y - fMean2);
	// 		float fCVar = (c - fMean2)*(c - fMean2);
	// 		float fMVar = (m - fMean2)*(m - fMean2);
	//
	// 		float fMaxPrimaryVar = max(fRVar, fGVar);
	// 		fMaxPrimaryVar = max(fMaxPrimaryVar, fBVar);
	//
	// 		float fMaxSecondaryVar = max(fCVar, fYVar);
	// 		fMaxSecondaryVar = max(fMaxSecondaryVar, fMVar);
	//
	// 		float fShading = 0.5f;
	//
	// 		auto compare = [&](float fV1, float fV2, float fC1, float fC2, short FG_LIGHT, short FG_DARK, short BG_LIGHT, short BG_DARK)
	// 		{
	// 			if (fV1 >= fV2)
	// 			{
	// 				// Primary Is Dominant, Use in foreground
	// 				fg_col = fC1 > 0.5f ? FG_LIGHT : FG_DARK;
	//
	// 				if (fV2 < 0.0001f)
	// 				{
	// 					// Secondary is not variant, Use greyscale in background
	// 					if (fC2 >= 0.00f && fC2 < 0.25f) bg_col = BG_BLACK;
	// 					if (fC2 >= 0.25f && fC2 < 0.50f) bg_col = BG_DARK_GREY;
	// 					if (fC2 >= 0.50f && fC2 < 0.75f) bg_col = BG_GREY;
	// 					if (fC2 >= 0.75f && fC2 <= 1.00f) bg_col = BG_WHITE;
	// 				}
	// 				else
	// 				{
	// 					// Secondary is varient, Use in background
	// 					bg_col = fC2 > 0.5f ? BG_LIGHT : BG_DARK;
	// 				}
	//
	// 				// Shade dominant over background (100% -> 0%)
	// 				fShading = ((fC1 - fC2) / 2.0f) + 0.5f;
	// 			}
	//
	// 			if (fShading >= 0.00f && fShading < 0.20f) sym = L' ';
	// 			if (fShading >= 0.20f && fShading < 0.40f) sym = PIXEL_QUARTER;
	// 			if (fShading >= 0.40f && fShading < 0.60f) sym = PIXEL_HALF;
	// 			if (fShading >= 0.60f && fShading < 0.80f) sym = PIXEL_THREEQUARTERS;
	// 			if (fShading >= 0.80f) sym = PIXEL_SOLID;
	// 		};
	//
	// 		if (fRVar == fMaxPrimaryVar && fYVar == fMaxSecondaryVar)
	// 			compare(fRVar, fYVar, r, y, FG_RED, FG_DARK_RED, BG_YELLOW, BG_DARK_YELLOW);
	//
	// 		if (fRVar == fMaxPrimaryVar && fMVar == fMaxSecondaryVar)
	// 			compare(fRVar, fMVar, r, m, FG_RED, FG_DARK_RED, BG_MAGENTA, BG_DARK_MAGENTA);
	//
	// 		if (fRVar == fMaxPrimaryVar && fCVar == fMaxSecondaryVar)
	// 			compare(fRVar, fCVar, r, c, FG_RED, FG_DARK_RED, BG_CYAN, BG_DARK_CYAN);
	//
	// 		if (fGVar == fMaxPrimaryVar && fYVar == fMaxSecondaryVar)
	// 			compare(fGVar, fYVar, g, y, FG_GREEN, FG_DARK_GREEN, BG_YELLOW, BG_DARK_YELLOW);
	//
	// 		if (fGVar == fMaxPrimaryVar && fCVar == fMaxSecondaryVar)
	// 			compare(fGVar, fCVar, g, c, FG_GREEN, FG_DARK_GREEN, BG_CYAN, BG_DARK_CYAN);
	//
	// 		if (fGVar == fMaxPrimaryVar && fMVar == fMaxSecondaryVar)
	// 			compare(fGVar, fMVar, g, m, FG_GREEN, FG_DARK_GREEN, BG_MAGENTA, BG_DARK_MAGENTA);
	//
	// 		if (fBVar == fMaxPrimaryVar && fMVar == fMaxSecondaryVar)
	// 			compare(fBVar, fMVar, b, m, FG_BLUE, FG_DARK_BLUE, BG_MAGENTA, BG_DARK_MAGENTA);
	//
	// 		if (fBVar == fMaxPrimaryVar && fCVar == fMaxSecondaryVar)
	// 			compare(fBVar, fCVar, b, c, FG_BLUE, FG_DARK_BLUE, BG_CYAN, BG_DARK_CYAN);
	//
	// 		if (fBVar == fMaxPrimaryVar && fYVar == fMaxSecondaryVar)
	// 			compare(fBVar, fYVar, b, y, FG_BLUE, FG_DARK_BLUE, BG_YELLOW, BG_DARK_YELLOW);
	// 	}
	//
	// 	return {sym, (WORD)(fg_col | bg_col)};
	//
	//}

	constexpr Pixel BLACK = {L' ', BG_BLACK};
	constexpr Pixel DARK_GREY = {L' ', BG_DARK_GREY};
	constexpr Pixel GREY = {L' ', BG_GREY};
	constexpr Pixel WHITE = {L' ', BG_WHITE};

	constexpr Pixel RED = {L' ', BG_RED};
	constexpr Pixel DARK_RED = {L' ', BG_DARK_RED};
	constexpr Pixel GREEN = {L' ', BG_GREEN};
	constexpr Pixel DARK_GREEN = {L' ', BG_DARK_GREEN};
	constexpr Pixel BLUE = {L' ', BG_BLUE};
	constexpr Pixel DARK_BLUE = {L' ', BG_DARK_BLUE};
	constexpr Pixel YELLOW = {L' ', BG_YELLOW};
	constexpr Pixel DARK_YELLOW = {L' ', BG_DARK_YELLOW};
	constexpr Pixel CYAN = {L' ', BG_CYAN};
	constexpr Pixel DARK_CYAN = {L' ', BG_DARK_CYAN};
	constexpr Pixel MAGENTA = {L' ', BG_MAGENTA};
	constexpr Pixel DARK_MAGENTA = {L' ', BG_DARK_MAGENTA};

	constexpr Pixel ORANGE = {PIXEL_HALF, FG_YELLOW | BG_RED};
	constexpr Pixel DARK_ORANGE = {PIXEL_HALF, FG_DARK_YELLOW | BG_DARK_RED};

} // namespace Pixels

struct VIRTUAL_KEY
{
	int keyCode;
	int state;
	int preState;

	VIRTUAL_KEY() = default;

	void Update()
	{
		preState = state;
		state = GetAsyncKeyState(keyCode);
	}

	inline bool Held()
	{
		return (state == -32767 || state == -32768);
	}
	inline bool Pressed()
	{
		return (preState == 0 && state == -32767);
	}
	inline bool Released()
	{
		return ((preState == -32767 || preState == -32768) && state == 0);
	}
};

class Image
{
private:
	Pixel *data = nullptr;
	int width = 0;
	int height = 0;

	void Init(int w, int h)
	{
		data = new Pixel[w * h];

		for (int i = 0; i < w * h; i++)
			data[i] = Pixels::BLACK;

		width = w;
		height = h;
	}

public:
	Image(int w, int h)
	{
		Init(w,h);
	}

	Image(std::wstring file)
	{
		if (!Load(file))
			Init(8,8);
	}

private:
	~Image()
	{
		if (data != nullptr)
			delete[] data;
	}

public:
	bool Load(std::wstring file)
	{
		int w = 0;
		int h = 0;
		if (data != nullptr)
			delete[] data;

		FILE *f;
		_wfopen_s(&f, file.c_str(), L"rb");

		if (f == nullptr)
			return false;

		fread((void *)&w, sizeof(int), 1, f);
		fread((void *)&h, sizeof(int), 1, f);

		Init(w,h);
		
		fread((void*)data, sizeof(Pixel), width * height, f);

		fclose(f);

		return true;
	}

	bool Save(std::wstring file)
	{
		FILE *f = nullptr;
		_wfopen_s(&f, file.c_str(), L"wb");

		if (f == nullptr)
			return false;

		fwrite((void *)&width, sizeof(int), 1, f);
		fwrite((void *)&height, sizeof(int), 1, f);
		fwrite((void *)data, sizeof(Pixel), width * height, f);

		fclose(f);

		return true;
	}

	inline int GetWidht() const { return width; }
	inline int GetHeight() const { return height; }
	inline Pixel* GetData() const { return data; }
	
	inline Pixel GetPixel(int i)
	{
		return data[i];
	}
	inline void SetPixel(int i, Pixel p)
	{
		data[i] = p;
	}

	Pixel GetPixel(int x, int y)
	{
		if (x >= 0 && x <= width && y >= 0 && y <= height)
			return data[y * width + x];
		else return Pixels::BLACK;
	}
	void SetPixel(int x, int y, Pixel p)
	{
		if (x >= 0 && x <= width && y >= 0 && y <= height)
			data[y * width + x] = p;
	}
	Pixel SamplePixel(float x, float y)
	{
		x = x - floorf(x);
		y = y - floorf(y);

		int sx = (int)((float)width * x);
		int sy = (int)((float)height * y);

		return data[sy * width + sx];
	}

};

class ConsoleApp
{
protected:
	HANDLE out = nullptr;
	HANDLE in = nullptr;
	Pixel *buffer;
	CONSOLE_FONT_INFOEX cfi;
	CONSOLE_CURSOR_INFO cci;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	CONSOLE_SCREEN_BUFFER_INFOEX csbix;
	VIRTUAL_KEY keys[MAX_KEYS];
	INPUT_RECORD records[32];

	std::wstring appName = L"Default";

	vs2d mouse = {(short)0, (short)0};
	vs2d pmouse = {(short)0, (short)0};
	int mouseHWheeled = 0;
	int mouseVWheeled = 0;

	bool hasFocus = true;

	int bufferLen = 0;
	short width = 0;
	short height = 0;

public:
	ConsoleApp() {}
	void Construct(bool useSTD = false)
	{
		if (useSTD)
			out = GetStdHandle(STD_OUTPUT_HANDLE);
		else
			out = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		SetConsoleActiveScreenBuffer(out);
		InitInfos();

		in = GetStdHandle(STD_INPUT_HANDLE);
		for (int i = 0; i < MAX_KEYS; i++)
		{
			keys[i].keyCode = i;
			keys[i].preState = 0;
			keys[i].state = GetAsyncKeyState(i);
		}
	}

	void Start()
	{
		std::thread t(&ConsoleApp::AppThread, this);
		t.join();
		Restore();
	}

protected:
	virtual bool Setup() = 0;
	virtual bool Update(float elapsedTime) = 0;
	virtual vs2d GetPreferredSize() = 0;
	virtual vs2d GetPreferredFont() = 0;

protected:
	bool SetFontName(const wchar_t *name)
	{
		wcscpy_s(cfi.FaceName, name);
		return SetCurrentConsoleFontEx(out, false, &cfi);
	}
	bool SetFontSize(short x, short y)
	{
		cfi.dwFontSize.X = x;
		cfi.dwFontSize.Y = y;
		return SetCurrentConsoleFontEx(out, false, &cfi);
	}
	bool SetFontWheight(int weight)
	{
		cfi.FontWeight = weight;
		return SetCurrentConsoleFontEx(out, false, &cfi);
	}
	bool SetBufferSize(short w, short h)
	{
		bool sucess = SetConsoleScreenBufferSize(out, {w, h});

		if (buffer != nullptr && sucess)
		{
			Pixel *newBuf = new Pixel[w * h];
			memset((void *)newBuf, 0, sizeof(Pixel) * w * h);
			memcpy_s((void *)newBuf, sizeof(Pixel) * w * h, (void *)buffer, sizeof(Pixel) * bufferLen);
			delete[] buffer;
			bufferLen = w * h;
			width = w;
			height = h;
			buffer = newBuf;
		}
		return sucess;
	}
	bool SetWindowSize(short w, short h)
	{
		csbi.srWindow.Right = w;
		csbi.srWindow.Bottom = h;
		return SetConsoleWindowInfo(out, true, &csbi.srWindow);
	}
	bool SetSize(short w, short h)
	{
		if (w < 2 || h < 2)
			return false;

		vs2d preBufSize = GetBufferSize();
		vs2d preWinSize = GetWindowSize();

		bool sucess = true;
		sucess = sucess && SetWindowSize(1, 1);
		sucess = sucess && SetBufferSize(w, h);
		sucess = sucess && SetWindowSize(w - 1, h - 1);

		if (!sucess)
		{
			SetBufferSize(preBufSize.x, preBufSize.y);
			SetWindowSize(preWinSize.x, preWinSize.y);
		}
		return sucess;
	}

	void SetResizable(bool value)
	{
		if (!value)
		{
			LONG theLong = GetWindowLongW(GetConsoleWindow(), GWL_STYLE);
			SetWindowLong(GetConsoleWindow(), GWL_STYLE, theLong & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
		}
		else
		{
			LONG theLong = GetWindowLong(GetConsoleWindow(), GWL_STYLE);
			SetWindowLong(GetConsoleWindow(), GWL_STYLE, theLong | WS_MAXIMIZEBOX | WS_SIZEBOX);
		}
	}
	void SetCursorVisible(bool value)
	{
		cci.bVisible = value;
		SetConsoleCursorInfo(out, &cci);
	}
	void EnableQuickEdit(bool value)
	{
		DWORD mode = 0;
		if (value)
		{
			GetConsoleMode(in, &mode);
			mode |= ENABLE_EXTENDED_FLAGS | ENABLE_QUICK_EDIT_MODE;
			SetConsoleMode(in, mode);
		}
		else
		{
			GetConsoleMode(in, &mode);
			mode &= ~ENABLE_QUICK_EDIT_MODE;
			SetConsoleMode(in, mode);
		}
	}
	void EnableMouseEvents(bool value)
	{
		DWORD mode = 0;
		if (value)
		{
			GetConsoleMode(in, &mode);
			mode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
			SetConsoleMode(in, mode);
		}
		else
		{
			GetConsoleMode(in, &mode);
			mode &= ~ENABLE_MOUSE_INPUT;
			SetConsoleMode(in, mode);
		}
	}
	void EnableWindowEvents(bool value)
	{
		DWORD mode = 0;
		if (value)
		{
			GetConsoleMode(in, &mode);
			mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
			SetConsoleMode(in, mode);
		}
		else
		{
			GetConsoleMode(in, &mode);
			mode &= ~ENABLE_WINDOW_INPUT;
			SetConsoleMode(in, mode);
		}
	}

	vs2d GetLargestWindowSize()
	{
		return GetLargestConsoleWindowSize(out);
	}
	vs2d GetWindowSize()
	{
		GetConsoleScreenBufferInfo(out, &csbi);
		return {csbi.srWindow.Right, csbi.srWindow.Bottom};
	}
	vs2d GetBufferSize()
	{
		GetConsoleScreenBufferInfo(out, &csbi);
		return csbi.dwSize;
	}

	void Restore()
	{
		if (GetStdHandle(STD_OUTPUT_HANDLE) != out)
		{
			SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
			CloseHandle(out);
			EnableQuickEdit(true);
			SetResizable(true);
			const int titleLen = 256;
			wchar_t originalTitle[titleLen];
			for (int i = 0; i < titleLen; i++)
				originalTitle[i] = L' ';

			GetConsoleOriginalTitleW(originalTitle, titleLen);
			SetConsoleTitleW(originalTitle);
		}
	}

protected:
	void Clear(Pixel p)
	{
		for (int i = 0; i < bufferLen; i++)
			buffer[i] = p;
	}
	virtual void Draw(int x, int y, Pixel p)
	{
		if (x >= 0 && x <= width && y >= 0 && y <= height)
			buffer[y * width + x] = p;
	}
	void DrawLine(int x1, int y1, int x2, int y2, Pixel p)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;
		double angle = atan2(dy, dx);
		double c = cos(angle);
		double s = sin(angle);
		double radius = sqrt(dx * dx + dy * dy);

		for (int i = 0; i < radius; i++)
			Draw((int)(c * i) + x1, (int)(s * i) + y1, p);
	}
	void FillRect(int x, int y, int w, int h, Pixel p)
	{
		for (int i = y; i < y + h; i++)
			for (int j = x; j < x + w; j++)
				Draw(j, i, p);
	}
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	void DrawImage(int x,int y, Image* img)
	{
		int w = img->GetWidht();
		int h = img->GetHeight();
		Pixel* data = img->GetData();

		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				Draw(x + j,y + i, data[i * w + j]);
				
	}

	// https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel p)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1 > y2)
		{
			SWAP(y1, y2);
			SWAP(x1, x2);
		}
		if (y1 > y3)
		{
			SWAP(y1, y3);
			SWAP(x1, x3);
		}
		if (y2 > y3)
		{
			SWAP(y2, y3);
			SWAP(x2, x3);
		}

		t1x = t2x = x1;
		y = y1; // Starting points
		dx1 = (int)(x2 - x1);
		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1);
		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
			signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1)
		{ // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2)
		{ // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2)
			goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;)
		{
			t1xp = 0;
			t2xp = 0;
			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}
			// process first line until y value is about to change
			while (i < dx1)
			{
				i++;
				e1 += dy1;
				while (e1 >= dx1)
				{
					e1 -= dx1;
					if (changed1)
						t1xp = signx1; //t1x += signx1;
					else
						goto next1;
				}
				if (changed1)
					break;
				else
					t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1)
			{
				e2 += dy2;
				while (e2 >= dx2)
				{
					e2 -= dx2;
					if (changed2)
						t2xp = signx2; //t2x += signx2;
					else
						goto next2;
				}
				if (changed2)
					break;
				else
					t2x += signx2;
			}
		next2:
			if (minx > t1x)
				minx = t1x;
			if (minx > t2x)
				minx = t2x;
			if (maxx < t1x)
				maxx = t1x;
			if (maxx < t2x)
				maxx = t2x;
			drawline(minx, maxx, y); // Draw line from min to max points found on the y
									 // Now increase y
			if (!changed1)
				t1x += signx1;
			t1x += t1xp;
			if (!changed2)
				t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2)
				break;
		}
	next:
		// Second half
		dx1 = (int)(x3 - x2);
		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1)
		{ // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++)
		{
			t1xp = 0;
			t2xp = 0;
			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}
			// process first line until y value is about to change
			while (i < dx1)
			{
				e1 += dy1;
				while (e1 >= dx1)
				{
					e1 -= dx1;
					if (changed1)
					{
						t1xp = signx1;
						break;
					} //t1x += signx1;
					else
						goto next3;
				}
				if (changed1)
					break;
				else
					t1x += signx1;
				if (i < dx1)
					i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3)
			{
				e2 += dy2;
				while (e2 >= dx2)
				{
					e2 -= dx2;
					if (changed2)
						t2xp = signx2;
					else
						goto next4;
				}
				if (changed2)
					break;
				else
					t2x += signx2;
			}
		next4:

			if (minx > t1x)
				minx = t1x;
			if (minx > t2x)
				minx = t2x;
			if (maxx < t1x)
				maxx = t1x;
			if (maxx < t2x)
				maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1)
				t1x += signx1;
			t1x += t1xp;
			if (!changed2)
				t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3)
				return;
		}
	}
	void DrawCircle(int xc, int yc, int r, Pixel p)
	{
		int x = 0;
		int y = r;
		int pt = 3 - 2 * r;
		if (!r)
			return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, p); //upper left left
			Draw(xc - y, yc - x, p); //upper upper left
			Draw(xc + y, yc - x, p); //upper upper right
			Draw(xc + x, yc - y, p); //upper right right
			Draw(xc - x, yc + y, p); //lower left left
			Draw(xc - y, yc + x, p); //lower lower left
			Draw(xc + y, yc + x, p); //lower lower right
			Draw(xc + x, yc + y, p); //lower right right
			if (pt < 0)
				pt += 4 * x++ + 6;
			else
				pt += 4 * (x++ - y--) + 10;
		}
	}
	void FillCircle(int xc, int yc, int r, Pixel p)
	{
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int pt = 3 - 2 * r;
		if (!r)
			return;

		auto drawline = [&](int sx, int ex, int ny) {
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, p);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (pt < 0)
				pt += 4 * x++ + 6;
			else
				pt += 4 * (x++ - y--) + 10;
		}
	};

	template <typename T>
	void DrawWireFrame(std::vector<v2d_generic<T>> model, T x, T y, T scale, T rotation, Pixel p)
	{
		int verts = model.size();
		std::vector<v2d_generic<T>> transformed;
		transformed.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			transformed[i].x = model[i].x * cos(rotation) - model[i].y * sin(rotation);
			transformed[i].y = model[i].x * sin(rotation) + model[i].y * cos(rotation);
			transformed[i] *= scale;
			transformed[i].x += x;
			transformed[i].y += y;
		}

		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(transformed[i % verts].x, transformed[i % verts].y, transformed[j % verts].x, transformed[j % verts].y, p);
			//Draw(transformed[i%verts].x, transformed[i%verts].y, p);
		}
	}

	template <typename T>
	void FillWireFrame(std::vector<v2d_generic<T>> model, T x, T y, T scale, T rotation, Pixel p)
	{
		int verts = model.size();
		std::vector<v2d_generic<T>> transformed;
		transformed.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			transformed[i].x = model[i].x * cos(rotation) - model[i].y * sin(rotation);
			transformed[i].y = model[i].x * sin(rotation) + model[i].y * cos(rotation);
			transformed[i] *= scale;
			transformed[i].x += x;
			transformed[i].y += y;
		}

		T minX = transformed[0].x;
		T maxX = transformed[0].x;
		T minY = transformed[0].y;
		T maxY = transformed[0].y;

		for (int i = 1; i < transformed.size(); i++)
		{
			v2d_generic<T> q = transformed[i];
			minX = min(q.x, minX);
			maxX = max(q.x, maxX);
			minY = min(q.y, minY);
			maxY = max(q.y, maxY);
		}

		for (int i = minY; i < maxY; i++)
			for (int j = minX; j < maxX; j++)
				if (IsPointInPolygon({(T)j, (T)i}, transformed))
					Draw(j, i, p);
	}

	template <typename T>
	bool IsPointInPolygon(v2d_generic<T> p, std::vector<v2d_generic<T>> &polygon)
	{
		T minX = polygon[0].x;
		T maxX = polygon[0].x;
		T minY = polygon[0].y;
		T maxY = polygon[0].y;
		for (int i = 1; i < polygon.size(); i++)
		{
			v2d_generic<T> q = polygon[i];
			minX = min(q.x, minX);
			maxX = max(q.x, maxX);
			minY = min(q.y, minY);
			maxY = max(q.y, maxY);
		}

		if (p.x < minX || p.x > maxX || p.y < minY || p.y > maxY)
			return false;

		bool inside = false;
		for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
		{
			if ((polygon[i].y > p.y) != (polygon[j].y > p.y) && p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)
				inside = !inside;
		}

		return inside;
	}

	template <typename T>
	void WrapAround(T ix, T iy, T &ox, T &oy)
	{
		ox = ix;
		oy = iy;
		if (ix < (T)0)
			ox = ix + (T)width;
		if (ix >= (T)width)
			ox = ix - (T)width;
		if (iy < (T)0)
			oy = iy + (T)height;
		if (iy >= (T)height)
			oy = iy - (T)height;
	}

	template <typename T>
	void Constrain(T ix, T iy, T &ox, T &oy)
	{
		ox = ix;
		oy = iy;
		if (ix < (T)0)
			ox = (T)0;
		if (ix >= (T)width)
			ox = (T)width;
		if (iy < (T)0)
			oy = (T)0;
		if (iy >= (T)height)
			oy = (T)height;
	}

private:
	void InitInfos()
	{
		cfi.cbSize = sizeof(cfi);
		csbix.cbSize = sizeof(csbix);
		GetCurrentConsoleFontEx(out, false, &cfi);
		cfi.FontFamily = FF_DONTCARE;
		SetCurrentConsoleFontEx(out, false, &cfi);
		GetConsoleScreenBufferInfo(out, &csbi);
		GetConsoleScreenBufferInfoEx(out, &csbix);
		GetConsoleCursorInfo(out, &cci);
	}

	void AppThread()
	{
		vs2d Font = GetPreferredFont();
		SetFontSize(Font.x, Font.y);

		vs2d Size = GetPreferredSize();

		bufferLen = Size.x * Size.y;
		buffer = new Pixel[bufferLen];
		memset((void *)buffer, 0, sizeof(Pixel) * bufferLen);
		SetSize(Size.x, Size.y);

		width = Size.x;
		height = Size.y;

		if (!Setup())
			return;

		bool running = true;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (running)
		{
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			for (int i = 0; i < MAX_KEYS; i++)
				keys[i].Update();

			DispatchEvents();

			if (!Update(fElapsedTime))
				running = false;

			pmouse = mouse;

			GetConsoleScreenBufferInfo(out, &csbi);
			WriteConsoleOutputW(out, buffer, csbi.dwSize, {0, 0}, &csbi.srWindow);
		}
	}

	void DispatchEvents()
	{
		DWORD num = 0;
		GetNumberOfConsoleInputEvents(in, &num);

		if (num > 32)
			num = 32;

		if (num > 0)
			ReadConsoleInputW(in, records, num, &num);

		for (DWORD i = 0; i < num; i++)
		{
			switch (records[i].EventType)
			{
			case MOUSE_EVENT:
				mouse = records[i].Event.MouseEvent.dwMousePosition;
				if (records[i].Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
				{
					short state = ((short)(((DWORD)(records[i].Event.MouseEvent.dwButtonState) >> 16) & 0xFFFF));
					mouseVWheeled = state > 0 ? 1 : -1;
				}
				if (records[i].Event.MouseEvent.dwEventFlags == MOUSE_HWHEELED)
				{
					short state = ((short)(((DWORD)(records[i].Event.MouseEvent.dwButtonState) >> 16) & 0xFFFF));
					mouseHWheeled = state > 0 ? 1 : -1;
				}
				break;
			case FOCUS_EVENT:
				hasFocus = records[i].Event.FocusEvent.bSetFocus;
				break;
			default:
				break;
			}
		}
	}
};