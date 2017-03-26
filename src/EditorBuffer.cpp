//
//  EditorBuffer.cpp
//  textEditor
//
//  Based on ofxEditor by by Darren Mothersele on 19/11/2013.
//  Modified by Joseph Wilk.
//
//
//

#include "EditorBuffer.h"

EditorBuffer::EditorBuffer(ofTrueTypeFont * _f) :
text(),
cursorPosition(text.end()),
selectStart(text.end()),
selectEnd(text.end()),
font(_f),
minScale(0.5),
maxScale(2.5)
{
  fontSize = 20;
  historyIdx = 0;
  history[0] = text;
  lineHeight = font->getLineHeight();
  charWidth = font->stringWidth("X") + font->getLetterSpacing();
  cursorPosition = text.begin();
  selectStart = cursorPosition;
  selectEnd = cursorPosition;
}

string EditorBuffer::getText() {
  return text;
}

void EditorBuffer::revertText(){
  historyIdx-=1;
  if(historyIdx < 0){
    historyIdx = 19;
  }
  string tmp = text;
  text = history[historyIdx];
  history[historyIdx] = tmp;

  if(cursorPosition > text.end()){
    cursorPosition = text.end();
  }
  if(cursorPosition < text.begin()){
    cursorPosition = text.begin();
  }
  selectStart = cursorPosition;
  selectEnd = cursorPosition;
}

void EditorBuffer::invert(){
  if(textColor == ofColor::white){
    textColor = ofColor::black;
    cursorColor = ofColor::black;
  }
  else{
    textColor = ofColor::white;
    cursorColor = ofColor::white;
  }
}

void EditorBuffer::storeTextChange(){
  history[historyIdx] = text;
  historyIdx += 1;
  if(historyIdx > 19){
    historyIdx = 0;
  }
}

void EditorBuffer::setText(string t) {
  text = t;
  cursorPosition = text.end();
  selectStart = cursorPosition;
  selectEnd = cursorPosition;
}

void EditorBuffer::insert(int key) {
  if (selectStart != selectEnd) {
    cursorPosition = text.erase(selectStart, selectEnd);
  }
  cursorPosition = text.insert(cursorPosition, key);
  cursorPosition++;
  updateSelect(false);
}

void EditorBuffer::insert(const string s) {
  int loc = cursorPosition - text.begin();
  text.insert(cursorPosition, s.begin(), s.end());
  cursorPosition = text.begin() + loc + s.size();
  updateSelect(false);
}

void EditorBuffer::updateSelect(bool shift) {
  if (shift) {
    if (cursorPosition > selectEnd) {
      selectEnd = cursorPosition;
    }
    else if (cursorPosition < selectStart) {
      selectStart = cursorPosition;
    }
  }
  else {
    selectStart = cursorPosition;
    selectEnd = cursorPosition;
  }
}

void EditorBuffer::del() {
  if (selectStart == selectEnd) {
    if (cursorPosition != text.end()) {
      if (cursorPosition != text.begin()) {
        cursorPosition = text.erase(cursorPosition);
      }
    }
  }
  else {
    cursorPosition = text.erase(selectStart, selectEnd);
  }
  updateSelect(false);
}

void EditorBuffer::backspace() {
  if (selectStart == selectEnd) {
    if (cursorPosition != text.begin()) {
      cursorPosition--;
      if (cursorPosition != text.end()) {
        cursorPosition = text.erase(cursorPosition);
      }
    }
  }
  else {
    if (cursorPosition != text.begin()) {
      cursorPosition = text.erase(selectStart, selectEnd);
    }
  }
  updateSelect(false);
}


void EditorBuffer::setCursorPosition(int c, int r) {
  if (c >= 0 && r >= 0) {
    int currentRow = 0;
    cursorPosition = text.begin();
    while (currentRow < r) {
      if (*cursorPosition == '\n') currentRow++;
      cursorPosition++;
    }
    int currentCol = 0;
    while (currentCol < c && *cursorPosition != '\n') {
      cursorPosition++;
      currentCol++;
    }
  }
  else {
    cursorPosition = text.begin();
  }
  // Prevent cursor from moving outside of text
  if (cursorPosition < text.begin()) cursorPosition = text.begin();
  if (cursorPosition > text.end()) cursorPosition = text.end();
}


void EditorBuffer::clear() {
  text.clear();
  cursorPosition = text.end();
  selectStart = cursorPosition;
  selectEnd = cursorPosition;
}


void EditorBuffer::moveCursorRow(int direction, bool shift, bool cmd) {
  setCursorPosition(getCurrentCol(), getCurrentRow() + direction);
  updateSelect(shift);
}

void EditorBuffer::moveCursorHome(bool shift, bool cmd) {
    setCursorPosition(0, 0);
    updateSelect(shift);
}

void EditorBuffer::moveCursorEnd(bool shift, bool cmd) {
    setCursorPosition(0, getEndRow());
    updateSelect(shift);
}

void EditorBuffer::moveCursorLineHome(bool shift, bool cmd) {
    bool notFound = true;
    int direction = -1;
    char stop_chars[] = "\n";

    for (unsigned int i = 0; i < strlen(stop_chars); ++i) {
        if (*(cursorPosition + direction) == stop_chars[i]) {
            notFound = false;
            break;
        }
    }
   
    while (notFound && cursorPosition > text.begin() && cursorPosition <= text.end()) {
        cursorPosition += direction;
        updateSelect(shift);

        for (unsigned int i = 0; i < strlen(stop_chars); ++i) {
            if (*(cursorPosition + direction) == stop_chars[i]) {
                notFound = false;
                break;
            }
        }


    }
}

void EditorBuffer::moveCursorLineEnd(bool shift, bool cmd) {
    bool notFound = true;
    int direction = 1;
    char stop_chars[] = "\n";
    
    for (unsigned int i = 0; i < strlen(stop_chars); ++i) {
        if (*(cursorPosition) == stop_chars[i]) {
            notFound = false;
            break;
        }
    }
    while (notFound && cursorPosition >= text.begin() && cursorPosition < text.end()) {
        for (unsigned int i = 0; i < strlen(stop_chars); ++i) {
            if (*(cursorPosition + direction) == stop_chars[i]) {
                notFound = false;
                break;
            }
        }
        cursorPosition += direction;
        updateSelect(shift);
    }
}


void EditorBuffer::moveCursorCol(int direction, bool shift, bool cmd) {
  if (cmd) {
    // If command key is pressed move to next word
    bool notFound = true;
    while (notFound && cursorPosition > text.begin() && cursorPosition < text.end()) {
      char stop_chars[] = " {}[]()\n";
      cursorPosition += direction;
      updateSelect(shift);
      for (unsigned int i = 0; i < strlen(stop_chars); ++i) {
        if (*(cursorPosition + direction) == stop_chars[i]) {
          notFound = false;
          break;
        }
      }
    }
  }
  else {
    if (direction == -1 && cursorPosition != text.begin()) {
      cursorPosition--;
      updateSelect(shift);
    }
    if (direction == 1 && cursorPosition != text.end()) {
      cursorPosition++;
      updateSelect(shift);
    }
  }
}

const string EditorBuffer::getSelection() {
  return string(selectStart, selectEnd);
}

void EditorBuffer::removeSelection() {
  cursorPosition = text.erase(selectStart, selectEnd);
  updateSelect(false);
}

string EditorBuffer::yank() {
  selectStart = cursorPosition;
  selectEnd   = cursorPosition;
  string yankedString = "";

  if(*(selectEnd) == '\n'){
    cursorPosition = text.erase(selectStart, selectEnd+1);
  }
  else{
    while (selectEnd >= text.begin() && selectEnd < text.end()) {
      selectEnd += 1;
      if (*(selectEnd) == '\n') {
        break;
      }

    }
    yankedString = string(selectStart, selectEnd);
    cursorPosition = text.erase(selectStart, selectEnd);
  }
  updateSelect(false);
  return yankedString;

}


int EditorBuffer::getEndRow() {
    // To get current row count number of '\n' characters between
    // text.begin and cursorPosition
    int rowNo = 0;
    for (string::iterator i = text.begin(); i < text.end(); i++) {
        if (*i == '\n') rowNo++;
    }
    return rowNo;
}

int EditorBuffer::getCurrentRow() {
  // To get current row count number of '\n' characters between
  // text.begin and cursorPosition
  int rowNo = 0;
  for (string::iterator i = text.begin(); i < cursorPosition; i++) {
    if (*i == '\n') rowNo++;
  }
  return rowNo;
}

int EditorBuffer::getCurrentCol() {
  // To get the current column count the number of characters between
  // cursorPosition and previous '\n' or text.begin()
  int colNo = 0;
  for (string::iterator i = cursorPosition - 1; i >= text.begin() && *i != '\n'; --i) {
    colNo++;
  }
  return colNo;
}


void EditorBuffer::setTextColor(ofColor _c1, ofColor _c2) {
  textColor = _c1;
  textBorderColor = _c2;
}
void EditorBuffer::setCursorColor(ofColor _c) {
  cursorColor = _c;
}
void EditorBuffer::setHighlightColor(ofColor _c) {
  highlightColor = _c;
}


void EditorBuffer::updateBounds() {
  bounds = ofRectangle(0, 0, 0, lineHeight);
  string ss;
  bool foundCursor = false;
  
  // keep track of selection areas
  shapes.clear();
  ofPath selection;
  ofPoint select;
  bool inHighlight = false;
  
  for (string::iterator i = text.begin(); i < text.end(); ++i) {
    if (i == cursorPosition) {
      cursorPoint = ofPoint(font->stringWidth(ss), bounds.height);
      foundCursor = true;
    }
    if (i == selectStart) {
      select = ofPoint(font->stringWidth(ss), bounds.height);
      select.y -= lineHeight;
      inHighlight = true;
    }
    if (i == selectEnd) {
      selection.rectangle(select.x, select.y, font->stringWidth(ss) - select.x, bounds.height - select.y);
      inHighlight = false;
    }
    if (*i == '\n') {
      if (inHighlight) {
        selection.rectangle(select.x, select.y, font->stringWidth(ss) - select.x, bounds.height - select.y);
      }
      bounds.growToInclude(font->stringWidth(ss), bounds.height);
      bounds.height += lineHeight;
      ss.clear();
      if (inHighlight) {
        select = ofPoint(font->stringWidth(ss), bounds.height);
        select.y -= lineHeight;
      }
    }
    else if (*i == ' ') {
      ss.push_back('_');
    }
    else {
      ss.push_back(*i);
    }
  }
  if (inHighlight) {
    selection.rectangle(select.x, select.y, font->stringWidth(ss) - select.x, bounds.height - lineHeight - select.y);
  }
  if (!foundCursor) {
    cursorPoint = ofPoint(font->stringWidth(ss), bounds.height);
  }
  bounds.growToInclude(font->stringWidth(ss), bounds.height);
  
  selection.setColor(highlightColor);
  shapes.push_back(selection);
}


void EditorBuffer::drawStrings() {
  float h = lineHeight;
  string token;
  int counter=0;
  string lineString;
  for (string::iterator i = text.begin(); i < text.end(); ++i) {
    if(token.compare("uniform") == 0){
      ofSetColor(ofColor::pink);
    }
    else if(token.compare("//") == 0){
      ofSetColor(ofColor::grey);
    }
    else if(token.compare("vec2") == 0 ||
       token.compare("vec3") == 0 ||
       token.compare("vec4") == 0 ||
       token.compare("float") == 0 ||
       token.compare("void") == 0 ||
       token.compare("int") == 0 ||
       token.compare("sampler2D") == 0
       ){
      ofSetColor(ofColor::green);
    }
    else if(token.compare("#define") == 0){
      ofSetColor(ofColor::grey);
    }
    else{
      ofSetColor(textColor);
    }

    if(*i != '\n'){
      token.push_back(*i);
      lineString.push_back(*i);
    }
    if(*i == ' ' || *i == '('){
      font->drawString(token, font->stringWidth(lineString) - font->stringWidth(token), h);
      token.clear();
    }
    else if(*i == '\n'){
      font->drawString(token, font->stringWidth(lineString) - font->stringWidth(token), h);
      token.clear();
      lineString.clear();
      h += lineHeight;
    }
  }
  font->drawString(token, font->stringWidth(lineString) - font->stringWidth(token), h);
}

void EditorBuffer::colorDrawStrings() {
  float h = lineHeight;
  string ss;
  for (string::iterator i = text.begin(); i < text.end(); ++i) {
    if (*i == '\n') {
      font->drawString(ss, 0, h);
      h += lineHeight;
      ss.clear();
    }
    else {
      ss.push_back(*i);
    }
  }
  font->drawString(ss, 0, h);
}

void EditorBuffer::drawCursor() {
  
  ofPushMatrix();
  ofPushStyle();
  //ofTranslate(x, y);
  
  ofPopStyle();
  ofPopMatrix();
}

void EditorBuffer::draw(float x, float y, float w, float h) {
  ofPushMatrix();
  ofPushStyle();
  ofTranslate(x, y);
  
  // Need to calculate size of editor before drawing
  updateBounds();
  
  // Scale down if needed
  float scale = 1;
  if (bounds.width > 0 && bounds.height > 0) {
    scale = min(w / bounds.width, h / bounds.height);
    scale = min(scale, maxScale);
    scale = max(scale, minScale);
    ofScale(scale, scale);
  }
  
  // Move editor content if cursor is off screen
  float offsetY = (h / scale) - cursorPoint.y;
  if (offsetY < 0) {
    ofTranslate(0, offsetY);
  }
  float offsetX = (w / scale) - cursorPoint.x;
  if (offsetX < 0) {
    ofTranslate(offsetX, 0);
  }
  
  // Draw selected text highlight
  for (vector<ofTTFCharacter>::iterator i = shapes.begin(); i < shapes.end(); ++i) {
    (*i).draw();
  }

  // Draw text buffer content
  ofSetColor(textColor);
  drawStrings();
  

  // Draw cursor
  ofSetColor(cursorColor);
  ofRect(cursorPoint, 10, -lineHeight);

  ofPopStyle();
  ofPopMatrix();
}

void EditorBuffer::updateShapes() {
  shapes.clear();
  ofPoint location(0, lineHeight);
  bool foundCursor = false;
  bool inHighlight = false;
  ofPoint select;
  ofPath selection;
  selection.setColor(highlightColor);
  ofPath cursor;
  cursor.setColor(cursorColor);
  string ss;

  for (string::iterator i = text.begin(); i < text.end(); ++i) {
    if (i == cursorPosition) {
      cursorPoint = ofPoint(location.x, location.y);
      cursor.rectangle(location.x, location.y, fontSize, -lineHeight);
      foundCursor = true;
    }
    if (i == selectStart) {
      select = location;
      select.y -= lineHeight;
      inHighlight = true;
    }
    if (i == selectEnd) {
      selection.rectangle(select.x, select.y, location.x - select.x, location.y - select.y);
      inHighlight = false;
    }
    if (*i == '\n') {
      font->drawString(ss, 0, location.y);
      ss.clear();
      if (inHighlight) {
        selection.rectangle(select.x, select.y, location.x - select.x, location.y - select.y);
      }
      location.x = 0;
      location.y += lineHeight;
      if (inHighlight) {
        select = location;
        select.y -= lineHeight;
      }
    }
    else if (*i == ' ') {
      ss.push_back(' ');
      location.x += charWidth;
    }
    else {
      /*
      ofTTFCharacter c = font->getCharacterAsPoints(*i);
      c.setStrokeColor(textBorderColor);
      c.setStrokeWidth(1);
      ofSetColor(textColor);
      shapes.push_back(c);
      shapes.back().translate(location);
      */
      //font->drawString(*i, location.x, location.y);
      ss.push_back(*i);
      location.x += charWidth;
    }
  }
  if (inHighlight) {
    selection.rectangle(select.x, select.y, location.x - select.x, location.y - select.y);
  }
  if (!foundCursor) {
    cursorPoint = ofPoint(location.x, location.y);
    //cursor.rectangle(location.x, location.y, 10, -lineHeight);
    cursor.rectangle(font->stringWidth(ss), location.y, fontSize, -lineHeight);
  }
  font->drawString(ss, 0, location.y);
  shapes.push_back(cursor);
  shapes.push_back(selection);

  bounds = ofRectangle(0,0,0,0);
  for (vector<ofTTFCharacter>::iterator i = shapes.begin(); i < shapes.end(); ++i) {
    //(*i).draw();
    //for (vector<ofPolyline>::iterator j = (*i).getOutline().begin(); j < (*i).getOutline().end(); ++j) {
      //bounds.growToInclude((*j).getBoundingBox());
    //}
  }
}



