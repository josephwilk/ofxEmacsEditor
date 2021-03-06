# ofxEmacsEditor

A simple barebones Emacs based text editor for openFrameworks for live coding.

Features:
* Multiple text buffers
* Syntax highlighting
* Copy paste (Mac only)
* Emacs navigation
* Undo

Based on [ofxEditor](https://github.com/darrenmothersele/ofxEditor) by [@darrenmothersele](https://github.com/darrenmothersele)

## Usage

Add to your openFrameworks project and initialize an editor variable passing
in the number of text buffers you require (defaults to 1). For example:

    class testApp : public ofBaseApp{

      ofxEmacsEditor editor;
      bool editorVisible;

    public:
      testApp() : editor(9) {}

      // Editor command callbacks are static methods
      static void toggleEditor(void *);

      // ... rest of your oF app ...

    };

You can define custom commands in your setup function, for example:

    void testApp::setup(){
      editor.addCommand('a', this, &testApp::toggleEditor);
    }

    void testApp::toggleEditor(void * _o) {
      ((testApp *)_o)->editorVisible = !((testApp *)_o)->editorVisible;
    }

To get the current text in the editor you need to reference the `getText()`
method of the current buffer.

    editor.buf[editor.currentBuffer]->getText();

For the editor to be visible you need to draw it in your main oF app draw loop:

    if (editorVisible) {
      editor.draw();
    }

