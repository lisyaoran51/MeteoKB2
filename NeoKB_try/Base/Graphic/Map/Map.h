#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define BRIGHTNESS_MAX 256


namespace Base {
namespace Graphic {
namespace Maps {

	class Map {

		uint8_t** matrix;

		uint8_t** defaultMatrix;

		int width;
		int height;

		bool clear;

	public:

		Map(int w, int h);

		~Map();

		uint8_t** GetMatrix();
		int GetWidth();
		int GetHeight();

		int Add(int x, int y, int v);

		int Set(int x, int y, int v);

		uint8_t Get(int x, int y);

		/// <summary>
		/// 將m上面x,y位置長寬xLen,yLen的圖案貼到這個map上，用疊加
		/// </summary>
		int PasteAdd(Map* m, int x, int y, int toX, int toY, int xLen, int yLen);

		/// <summary>
		/// 將m上面x,y位置長寬xLen,yLen的圖案貼到這個map上，用重社
		/// </summary>
		int PasteSet(Map* m, int x, int y, int toX, int toY, int xLen, int yLen);

		/// <summary>
		/// 每一次frame都要先把map重置一次再開始畫
		/// </summary>
		int Reset();

		bool IsClear();

		/// <summary>
		/// set the map now as the default map. after reset the map, the default map value would be the map now
		/// </summary>
		virtual int SetDefault();
	};

	/*
	template<typename T>
	struct Matrix2D {
		T** mat;
		int width;
		int height;
		Matrix2D(int w, int h);
		int Clean();
	};
	*/


}}}




#endif