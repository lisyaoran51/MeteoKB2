#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <string>

using namespace std;

namespace Framework {
namespace Graphic {


	class Drawable {

		string name;

		int positionX, positionY;
		float rotation;
		Anchor anchor;
		float alpha;
		float transparency;
		int width, height;

		bool isMasked;
		int depth;

		Map* graph;

	public:
		Drawable(int w, int h);
		
		int SetPositionX(int x);
		int SetPositionY(int y);
		int GetPositionX();
		int GetPositionY();
		
		int SetRotation(float r);
		float GetRotation();

		int SetAnchor(Anchor a);
		Anchor GetAnchor();

		int SetAlpha(float a);
		float GetAlpha();

		int SetTransparency(float t);
		float GetTransparency();

		int SetWidth(int w);
		int SetHeight(int h);
		int GetWidth();
		int GetHeight();

		int SetIsMasked(bool value);
		bool GetIsMasked();
		int SetDepth(int d);
		int GetDepth();

		Map* GetGraph();
	};

	enum class Anchor {
		TopLeft,
		TopCentre,
		TopRight,

		CentreLeft,
		Centre,
		CentreRight,

		BottomLeft,
		BottomCentre,
		BottomRight,
	};

}}





#endif