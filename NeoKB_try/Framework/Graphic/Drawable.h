#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <string>
#include "Map/Map.h"

using namespace std;
using namespace Framework::Graphic::Maps;

namespace Framework {
namespace Graphic {

	enum class Anchor;

	class Drawable {

		string name;
		bool isInitialized;
		bool isDrawable;

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
		Drawable();
		Drawable(int w, int h);
		~Drawable();
		
		int Initialize(int w, int h);
		int Deactivate();

		bool GetIsDrawable();

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

		virtual Map* GetGraph();

	protected:

		int SetIsDrawable(bool value);

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