//
//  EditorBuffer.h
//  textEditor
//
//  Based on ofxEditor by by Darren Mothersele on 19/11/2013.
//  Modified by Joseph Wilk.
//
//
//

#ifndef __textEditor__EditorBuffer__
#define __textEditor__EditorBuffer__

#include "ofMain.h"

class EditorBuffer {

  string text;
  string::iterator cursorPosition;
  string::iterator selectStart;
  string::iterator selectEnd;
  void updateSelect(bool);

  ofColor textColor;
  ofColor textBorderColor;
  ofColor cursorColor;
  ofColor highlightColor;

  
  
  ofTrueTypeFont * font;

	vector<ofTTFCharacter> shapes;
  ofRectangle bounds;
  ofPoint cursorPoint;
  float minScale, maxScale;
  
public:
  float lineHeight;
  float charWidth;
  int fontSize;
  EditorBuffer(ofTrueTypeFont * f);

  void insert(int);
  void insert(const string);
  void backspace();
  void del();
  void clear();
  string yank();

  string getText();
  void setText(string);

  void moveCursorHome(bool shift, bool cmd);
  void moveCursorEnd(bool shift, bool cmd);


  void moveCursorLineHome(bool shift, bool cmd);
  void moveCursorLineEnd(bool shift, bool cmd);

  void moveCursorRow(int direction, bool shift, bool cmd);
  void moveCursorCol(int direction, bool shift, bool cmd);

  void setTextColor(ofColor, ofColor);
  void setCursorColor(ofColor);
  void setHighlightColor(ofColor);

  const string getSelection();
  void removeSelection();

  void setCursorPosition(int, int);

  int getEndRow();
  int getCurrentRow();
  int getCurrentCol();

  void draw(float, float, float, float);
  ofRectangle getBoundingBox();
  void updateShapes();
  void updateBounds();
  void drawStrings();

  void colorDrawStrings();

  void drawCursor();
  
};

#endif /* defined(__textEditor__EditorBuffer__) */
