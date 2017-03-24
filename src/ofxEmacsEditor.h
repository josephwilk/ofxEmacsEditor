//
//  ofxEmacsEditor.h
//  textEditor
//
//  Based on ofxEditor by by Darren Mothersele on 19/11/2013.
//  Modified by Joseph Wilk.
//
//

#ifndef __textEditor__ofxEditor__
#define __textEditor__ofxEditor__

#include "ofMain.h"
#include "ClipBoard.h"
#include "EditorBuffer.h"

typedef void (*EditorCommand)(void *);

class ofxEmacsEditor {

  ofFbo editorFbo;
  ofTrueTypeFont font;

  bool selectMode;
  string fontName;
  int fontSize;
  ofColor textColor;
  ofColor textBorderColor;
  ofColor cursorColor;
  ofColor highlightColor;

  map<int, pair<void *, EditorCommand> > cmds;

public:
  ofxEmacsEditor(int noBuffers = 1, string fontname = "DroidSansMono.ttf");

  void addCommand(int key, void *_o, EditorCommand _c) {
    cmds[key] = make_pair(_o, _c);
  }

  void handleKeyPress(ofKeyEventArgs &);

  void draw();
  void update();

  void reloadFonts();
  void windowResized(ofResizeEventArgs &);

  vector<EditorBuffer*> buf;
  int currentBuffer;
  int maxBuffer;
  
  bool loadFile(string filename, int whichBuffer);
  bool saveFile(string filename, int whichBuffer);
  
};


#endif /* defined(__textEditor__ofxEmacsEditor__) */
