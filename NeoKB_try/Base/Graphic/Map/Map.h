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
		/// �Nm�W��x,y��m���exLen,yLen���Ϯ׶K��o��map�W�A���|�[
		/// </summary>
		int PasteAdd(Map* m, int x, int y, int toX, int toY, int xLen, int yLen);

		/// <summary>
		/// �Nm�W��x,y��m���exLen,yLen���Ϯ׶K��o��map�W�A�έ���
		/// </summary>
		int PasteSet(Map* m, int x, int y, int toX, int toY, int xLen, int yLen);

		/// <summary>
		/// �C�@��frame���n����map���m�@���A�}�l�e
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